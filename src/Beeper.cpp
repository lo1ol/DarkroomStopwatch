#include "Beeper.h"

#include <Arduino.h>

#include "Tools.h"

constexpr int kVol = 255;
constexpr uint16_t kAlarmMelody[] = { 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50 };

Beeper::Beeper() {
    pinMode(BEEPER, OUTPUT);
}

void Beeper::shutUp() {
    m_play = false;
    m_play = false;
    analogWrite(BEEPER, 0);
}

void Beeper::play(Style style) {
    m_play = true;
    m_style = style;
    m_melodyPhase = 0;
    m_timer = millis() + (style == Beep ? 50 : kAlarmMelody[0]);
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
        if (m_timer > currentTime)
            break;
        ++m_melodyPhase;
        analogWrite(BEEPER, m_melodyPhase & 1 ? 0 : kVol);

        if (m_melodyPhase == sizeof(kAlarmMelody) / sizeof(kAlarmMelody[0]))
            shutUp();
        else
            m_timer = currentTime + kAlarmMelody[m_melodyPhase];
        break;
    }
}

