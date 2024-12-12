#pragma once

#define EB_NO_FOR
#define EB_NO_CALLBACK
#define EB_NO_COUNTER
#define EB_NO_BUFFER
#define EB_FAST_TIME 15 // таймаут быстрого поворота (энкодер)

#include <EncButton.h>

#include <stdint.h>

#include "Config.h"

class Hardware {
public:
    Hardware();
    void tick();

    bool getInt(uint8_t& val, uint8_t min = 0, uint8_t max = 60);

    bool startClick();
    bool resetClick();
    bool resetHold();

    void effectiveMode(bool);

    static void wakeUp();

private:
    void enableHardware();
    void disableHardware();

    void sleep();

    int8_t getEncoderDir();
    void updateTurnOffTime();

    uint32_t m_lastReponseTime = 0;
    bool m_allowSleep = false;
    bool m_disabled = false;
    bool m_prepareForSleep = false;
    bool m_ignoreBtns = false;

    ButtonT<RESET_BTN> m_resetBtn;
    ButtonT<START_BTN> m_startBtn;
};
