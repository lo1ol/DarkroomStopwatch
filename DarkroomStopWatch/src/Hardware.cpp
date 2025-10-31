#include "Hardware.h"

#include <Arduino.h>

#include "Tools.h"

#include <GyverPower.h>

#include <PinChangeInterrupt.h>

namespace {

EncButton gEncoder{ ENCODER_DT, ENCODER_CLK };

void processISR() {
    gEncoder.tickISR();
}

} // namespace

void Hardware::init() {
    gEncoder.setEncISR(true);

    power.setSleepMode(POWERDOWN_SLEEP);
    power.autoCalibrate();
    power.setSleepResolution(SLEEP_8192MS);

    pinMode(ENCODER_PWR, OUTPUT);
    digitalWrite(ENCODER_PWR, HIGH);
    pinMode(DISPLAY_PWR, OUTPUT);
    enableHardware();

    attachInterrupt(0, processISR, CHANGE);
    attachInterrupt(1, processISR, CHANGE);
}

void Hardware::tick() {
    gEncoder.tick();
    m_startBtn.tick();
    m_resetBtn.tick();
    m_settingBtn.tick(m_startBtn, m_resetBtn);

    m_justWakedUp = false;
    m_justWakedUpAfterDeepSleep = false;

    if (m_resetBtn.pressing() || m_startBtn.pressing() || gEncoder.turn()) {
        updateTurnOffTime();

        if (gBeeper.isPlay() && gBeeper.style() == Beeper::LongAlarm) {
            gBeeper.shutUp();
            m_ignoreBtns = true;
        }

        if (m_disabled) {
            wakeUp();
            return;
        }
    }

    // if nothing is pressed and we still ignore btns
    if (m_ignoreBtns && gMillis() - m_sleepTime > 100 && !m_resetBtn.busy() && !m_startBtn.busy()) {
        // stop ignore them
        m_ignoreBtns = false;
    }

    if (m_ignoreBtns)
        return;

    if (m_startBtn.hold()) {
        gDisplay.power(false);
        digitalWrite(DISPLAY_PWR, LOW);
        m_prepareForSleep = true;
        return;
    }

    if (m_prepareForSleep && m_startBtn.release()) {
        m_prepareForSleep = false;
        m_disabled = true;

        if (m_allowSleep)
            sleep();
        return;
    }

    if (m_allowSleep && gMillis() > m_sleepTime) {
        sleep();
    }
}

void Hardware::wakeUp() {
    if (!gHardware.m_disabled)
        return;

    digitalWrite(DISPLAY_PWR, HIGH);
    gDisplay.power(true);

    gHardware.m_disabled = false;
    gHardware.m_ignoreBtns = true;

    if (power.inSleep())
        power.wakeUp();

    gHardware.updateTurnOffTime();
}

void Hardware::sleep() {
    m_disabled = true;
    disableHardware();
    power.sleepDelay(60 * 60 * 1000L);
    if (m_disabled) {
        power.sleep(SLEEP_FOREVER);
    }
    enableHardware();
    m_justWakedUp = true;
}

int8_t Hardware::getEncoderDir() {
    if (!gEncoder.turn() || (m_ignoreBtns && gMillis() - m_sleepTime < 100))
        return 0;

    return gEncoder.dir();
}

bool Hardware::getInt(uint8_t& val_, uint8_t min, uint8_t max, bool bound) {
    int16_t val = val_;
    int8_t shift{ getEncoderDir() };
    if (gEncoder.fast() && max - min > 20)
        shift *= 5;

    val += shift;

    if (bound) {
        if (val < min)
            val = min;
        if (val >= max)
            val = max - 1;
    } else {
        if (val < min)
            val += max - min;

        if (val >= max || val < min)
            val = min + ((val - min) % (max - min));
    }

    val_ = val;
    return shift;
}

bool Hardware::getBool(bool& val_) {
    uint8_t val = val_;
    auto res = getInt(val, 0, 2, true);

    val_ = val;
    return res;
}

bool Hardware::startClick() {
    return !m_ignoreBtns && m_startBtn.click();
}

bool Hardware::resetClick() {
    return !m_ignoreBtns && m_resetBtn.click();
}

bool Hardware::resetHold() {
    return !m_ignoreBtns && m_resetBtn.hold();
}

bool Hardware::settingHold() {
    return !m_ignoreBtns && m_settingBtn.hold();
}

void Hardware::disableHardware() {
    gDisplay.power(false);
    digitalWrite(DISPLAY_PWR, LOW);

    attachPinChangeInterrupt(digitalPinToPinChangeInterrupt(ENCODER_DT), Hardware::wakeUp, CHANGE);
    attachPinChangeInterrupt(digitalPinToPinChangeInterrupt(START_BTN), Hardware::wakeUp, FALLING);
    attachPinChangeInterrupt(digitalPinToPinChangeInterrupt(RESET_BTN), Hardware::wakeUp, FALLING);

    power.hardwareDisable(PWR_ALL);
}

void Hardware::enableHardware() {
    digitalWrite(DISPLAY_PWR, HIGH);
    gDisplay.power(true);

    power.hardwareEnable(PWR_ALL);
    power.hardwareDisable(PWR_I2C | PWR_SPI | PWR_USB | PWR_ADC | PWR_TIMER1 | PWR_TIMER2 | PWR_UART1 | PWR_UART2 |
                          PWR_UART3);

    detachPinChangeInterrupt(digitalPinToPinChangeInterrupt(ENCODER_DT));
    detachPinChangeInterrupt(digitalPinToPinChangeInterrupt(START_BTN));
    detachPinChangeInterrupt(digitalPinToPinChangeInterrupt(RESET_BTN));

    updateTurnOffTime();
}

void Hardware::updateTurnOffTime() {
    m_sleepTime = AUTOSLEEP_TIME_SEC * MS_IN_SEC + gMillis();
}

void Hardware::effectiveMode(bool effective) {
    // we need to stop interruptions during changing prescale
    noInterrupts();
    power.setSystemPrescaler(effective ? PRESCALER_1 : PRESCALER_4);
    interrupts();

    // enc button doesn't support prescaling
    // so, we have to change timepouts manually
    uint8_t debTime = 50;
    uint16_t clickTime = 200;
    uint16_t holdTime = 500;

    if (!effective) {
        debTime >>= CLKPR & 0xF;
        clickTime >>= CLKPR & 0xF;
        holdTime >>= CLKPR & 0xF;
    }

    m_startBtn.setDebTimeout(debTime);
    m_startBtn.setClickTimeout(max(1, clickTime));
    m_startBtn.setHoldTimeout(max(1, holdTime));

    m_resetBtn.setDebTimeout(debTime);
    m_resetBtn.setClickTimeout(clickTime);
    m_resetBtn.setHoldTimeout(holdTime);

    m_allowSleep = effective;
    updateTurnOffTime();
}
