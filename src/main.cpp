#include "Tools.h"

#include <GyverPower.h>

namespace {

enum class Mode {
    set,
    runTime,
    stopwatch,
    stopwatchPause,
};

Mode gMode = Mode::set;
bool gSetMin = true;

uint8_t gRunningMin = 0;
uint8_t gRunningSec = 0;
uint32_t gStartTime = 0;
uint32_t gStopTime = 0;
uint32_t gPrevPassedTime = 0;

uint16_t gNextNotifyTime;
bool gLongAlarm;

void changeMode(Mode mode) {
    gBeeper.shutUp();

    switch (mode) {
    case Mode::set:
        gHardware.effectiveMode(true);
        if (gPrevPassedTime) {
            gRunningMin = (gPrevPassedTime / MS_IN_SEC) / 60;
            gRunningSec = (gPrevPassedTime / MS_IN_SEC) % 60;
        }
        gPrevPassedTime = 0;

        if (gSetMin)
            gDisplay.blinkMin(false);
        else
            gDisplay.blinkSec(false);
        break;
    case Mode::runTime:
        gHardware.effectiveMode(false);
        gBeeper.play(Beeper::Beep);
        gStopTime = millis() + (gRunningMin * 60L + gRunningSec) * MS_IN_SEC;
        gNextNotifyTime = (gRunningMin > 0 && gRunningMin < 6) ? 10 : -1;
        gLongAlarm = gRunningMin >= 20;
        gStartTime = millis();
        gDisplay.resetBlink();
        break;
    case Mode::stopwatch:
        gHardware.effectiveMode(false);
        gBeeper.play(Beeper::Beep);
        gStartTime = millis() - gPrevPassedTime;
        gPrevPassedTime = 0;
        gDisplay.resetBlink();
        break;
    case Mode::stopwatchPause:
        gPrevPassedTime = millis() - gStartTime;
        gHardware.effectiveMode(true);
        gBeeper.play(Beeper::Beep);
        gDisplay.blinkDots(false);
        break;
    }

    gMode = mode;
}

} // namespace

void setup() {
    changeMode(Mode::set);
}

void loop() {
    gDisplay.tick();
    gHardware.tick();
    gBeeper.tick();

    if (gHardware.justWakedUp()) {
        changeMode(Mode::set);
        return;
    }

    auto curTime = millis();
    auto runningTime = gRunningMin * 60L + gRunningSec;

    if (gHardware.resetHold()) {
        if (gMode == Mode::set || gMode == Mode::stopwatchPause) {
            gPrevPassedTime = 0;
            gRunningMin = 0;
            gRunningSec = 0;
        }

        changeMode(Mode::set);
        return;
    }

    switch (gMode) {
    case Mode::set:
        if (gSetMin && gHardware.getInt(gRunningMin))
            gDisplay.blinkMin(true);

        if (!gSetMin && gHardware.getInt(gRunningSec))
            gDisplay.blinkSec(true);

        gDisplay.showTime(runningTime);

        if (gHardware.startClick()) {
            changeMode(runningTime == 0 ? Mode::stopwatch : Mode::runTime);
            break;
        }

        if (gHardware.resetClick()) {
            gSetMin = !gSetMin;
            if (gSetMin)
                gDisplay.blinkMin(false);
            else
                gDisplay.blinkSec(false);
            break;
        }

        break;
    case Mode::runTime: {
        if (curTime > gStopTime) {
            changeMode(Mode::set);
            gBeeper.play(gLongAlarm ? Beeper::LongAlarm : Beeper::Alarm);
            break;
        }

        auto restTime = (gStopTime - curTime) / MS_IN_SEC + 1;
        if (gNextNotifyTime == restTime) {
            gBeeper.play(Beeper::ShortAlarm);
            if (gNextNotifyTime == 10)
                gNextNotifyTime = 0;
        }

        gDisplay.showTime(restTime);
        break;
    }
    case Mode::stopwatch: {
        auto passTime = (curTime - gStartTime) / MS_IN_SEC;
        if (passTime >= 3600) {
            changeMode(Mode::set);
            gBeeper.play(Beeper::LongAlarm);
            break;
        }
        gDisplay.showTime(passTime);

        if (gHardware.startClick()) {
            changeMode(Mode::stopwatchPause);
            break;
        }

        break;
    }
    case Mode::stopwatchPause:
        gDisplay.showTime(gPrevPassedTime / MS_IN_SEC);

        if (gHardware.startClick()) {
            changeMode(Mode::stopwatch);
            break;
        }

        if (gHardware.resetClick() || (gSetMin && gHardware.getInt(gRunningMin)) ||
            (!gSetMin && gHardware.getInt(gRunningSec))) {
            changeMode(Mode::set);
            break;
        }

        break;
    }
}
