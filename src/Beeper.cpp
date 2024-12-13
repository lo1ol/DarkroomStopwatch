#include "Beeper.h"

#include <Arduino.h>

#include "Tools.h"

constexpr int kVol = 255;

Beeper::Beeper() {
    pinMode(BEEPER, OUTPUT);
}

void Beeper::shutUp() {
    m_play = false;
    analogWrite(BEEPER, 0);
}

void Beeper::play(Style style) {
    m_play = true;
    m_style = style;
    m_melodyPhase = 0;
    m_timer = millis() + (style == LongAlarm ? 500 : 50);
    analogWrite(BEEPER, kVol);
}

void Beeper::tick() {
    if (!m_play)
        return;

    uint32_t currentTime = millis();

    switch (m_style) {
    case Beep:
        if (m_timer < currentTime) {
            shutUp();
        }
        break;
    case Alarm:
    case ShortAlarm:
    case LongAlarm:
        if (m_timer > currentTime)
            break;
        ++m_melodyPhase;
        analogWrite(BEEPER, m_melodyPhase & 1 ? 0 : kVol);

        if (m_style == Alarm) {
            if (m_melodyPhase == 20)
                shutUp();
            else
                m_timer = currentTime + 50;
        }

        if (m_style == ShortAlarm) {
            if (m_melodyPhase == 6)
                shutUp();
            else
                m_timer = currentTime + 50;
        }

        if (m_style == LongAlarm) {
            if (m_melodyPhase == 60)
                shutUp();
            else
                m_timer = currentTime + 500;
        }
        break;
    }
}

