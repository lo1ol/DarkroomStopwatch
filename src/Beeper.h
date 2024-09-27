#pragma once

#include <stdint.h>

class Beeper {
public:
    enum Style {
        Beep,
        Alarm,
    };

    Beeper();

    void tick();
    void play(Style);
    void shutUp();

private:
    bool m_play = false;
    Style m_style;
    uint8_t m_melodyPhase = 0;
    uint32_t m_timer = 0;
};
