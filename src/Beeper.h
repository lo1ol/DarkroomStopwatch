#pragma once

#include <stdint.h>

class Beeper {
public:
    enum Style {
        Beep,
        ShortAlarm,
        Alarm,
        LongAlarm,
    };

    Beeper();

    void tick();
    void play(Style);
    void shutUp();

    bool isPlay() const { return m_play; }
    Style style() const { return m_style; }

private:
    bool m_play = false;
    Style m_style;
    uint16_t m_melodyPhase = 0;
    uint32_t m_timer = 0;
};
