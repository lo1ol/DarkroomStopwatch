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

Hardware::Hardware() {
    gEncoder.setEncISR(true);

    power.setSleepMode(POWERDOWN_SLEEP);
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
    if (m_ignoreBtns && millis() - m_lastReponseTime > 100 && !m_resetBtn.busy() && !m_startBtn.busy()) {
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

    if (m_allowSleep && millis() > m_lastReponseTime) {
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
    power.sleep(SLEEP_FOREVER);
    enableHardware();
}

int8_t Hardware::getEncoderDir() {
    if (!gEncoder.turn() || (m_ignoreBtns && millis() - m_lastReponseTime < 100))
        return 0;

    return gEncoder.dir();
}

bool Hardware::getInt(uint8_t& val_, uint8_t min, uint8_t max) {
    int8_t val = val_;
    int8_t shift{ getEncoderDir() };
    if (gEncoder.fast())
        shift *= 5;

    val += shift;

    if (val < 0)
        val += max - min;

    if (val >= max || val < min)
        val = min + (val % (max - min));

    val_ = val;
    return shift;
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

void Hardware::disableHardware() {
    digitalWrite(DISPLAY_PWR, LOW);
    gDisplay.power(false);

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
    m_lastReponseTime = 5 * 60 * MS_IN_SEC + millis();
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
