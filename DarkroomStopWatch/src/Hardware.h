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
    Hardware() = default;
    void init();

    void tick();

    bool getInt(uint8_t& val, uint8_t min = 0, uint8_t max = 60, bool bound = false);
    bool getBool(bool& val);

    bool startClick();
    bool resetClick();
    bool resetHold();
    bool settingHold();

    void effectiveMode(bool);

    bool justWakedUp() const { return m_justWakedUp; }
    bool justWakedUpAfterDeepSleep() const { return m_justWakedUpAfterDeepSleep; }

private:
    void enableHardware();
    void disableHardware();

    static void wakeUp();

    void sleep();

    int8_t getEncoderDir();
    void updateTurnOffTime();

    uint32_t m_sleepTime = 0;
    bool m_allowSleep = false;
    bool m_disabled = false;
    bool m_prepareForSleep = false;
    bool m_ignoreBtns = false;
    bool m_needOpenSetting = false;
    bool m_justWakedUp = true;
    bool m_justWakedUpAfterDeepSleep = true;

    ButtonT<RESET_BTN_PIN> m_resetBtn;
    ButtonT<START_BTN_PIN> m_startBtn;
    VirtButton m_settingBtn;
};
