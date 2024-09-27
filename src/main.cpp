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

void changeMode(Mode mode) {
    gBeeper.shutUp();

    switch (mode) {
    case Mode::set:
        gHardware.effectiveMode(true);
        if (gPrevPassedTime) {
            gRunningMin = (gPrevPassedTime / 1000) / 60;
            gRunningSec = (gPrevPassedTime / 1000) % 60;
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
        gStopTime = millis() + (gRunningMin * 60L + gRunningSec) * 1000L;
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
    case Mode::runTime:
        if (curTime > gStopTime) {
            changeMode(Mode::set);
            gBeeper.play(Beeper::Alarm);
            break;
        }

        gDisplay.showTime((gStopTime - curTime) / 1000);
        break;
    case Mode::stopwatch:
        if ((curTime - gStartTime) / 1000 > 3600) {
            changeMode(Mode::set);
            gBeeper.play(Beeper::Alarm);
            break;
        }
        gDisplay.showTime((curTime - gStartTime) / 1000);

        if (gHardware.startClick()) {
            changeMode(Mode::stopwatchPause);
            break;
        }

        break;
    case Mode::stopwatchPause:
        gDisplay.showTime(gPrevPassedTime / 1000);

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
