#pragma once

#include <GyverSegment.h>

#include "Config.h"

class Display {
public:
    Display();

    void tick();

    void showTime(uint16_t time);

    void blinkMin(bool initState);
    void blinkSec(bool initState);
    void blinkDots(bool initState);

    void resetBlink();

    void power(bool v) { m_display.power(v); }

private:
    void update();

    bool m_blinkMin = false;
    bool m_blinkSec = false;
    bool m_blinkDots = false;
    bool m_blinkState = true;
    uint32_t m_blinkTimer = 0;

    uint16_t m_time = -1;

    Disp1637Colon m_display;
};
