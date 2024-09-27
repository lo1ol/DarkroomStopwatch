#include "Display.h"

#include <Arduino.h>

constexpr uint16_t kBlinkTime = 500;

constexpr uint8_t kSegCharMap[] = {
    0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F,
};

Display::Display() : m_display(DISPLAY_DIO, DISPLAY_CLK, true) {
    m_display.brightness(0);
}

void Display::showTime(uint16_t time) {
    if (m_time == time)
        return;
    m_time = time;
    update();
}

void Display::update() {
    uint8_t secs = m_time % 60;
    uint8_t mins = m_time / 60;

    m_display.buffer[0] = 0;
    m_display.buffer[1] = 0;
    m_display.buffer[2] = 0;
    m_display.buffer[3] = 0;
    if (!m_blinkMin || m_blinkState) {
        m_display.buffer[0] = kSegCharMap[mins / 10];
        m_display.buffer[1] = kSegCharMap[mins % 10];
    }

    if (!m_blinkSec || m_blinkState) {
        m_display.buffer[2] = kSegCharMap[secs / 10];
        m_display.buffer[3] = kSegCharMap[secs % 10];
    }

    m_display.colon(!m_blinkDots || m_blinkState);
    m_display.update();
}

void Display::tick() {
    uint32_t ms = millis();
    if ((m_blinkMin || m_blinkSec || m_blinkDots) && m_blinkTimer < ms) {
        m_blinkState = !m_blinkState;
        m_blinkTimer = ms + kBlinkTime;
        update();
    }
}

void Display::blinkMin(bool initState) {
    m_blinkDots = false;
    m_blinkSec = false;
    m_blinkMin = true;
    m_blinkState = initState;
    m_blinkTimer = millis() + kBlinkTime;
    update();
}

void Display::blinkSec(bool initState) {
    m_blinkDots = false;
    m_blinkMin = false;
    m_blinkSec = true;
    m_blinkState = initState;
    m_blinkTimer = millis() + kBlinkTime;
    update();
}

void Display::blinkDots(bool initState) {
    m_blinkMin = false;
    m_blinkSec = false;
    m_blinkDots = true;
    m_blinkState = initState;
    m_blinkTimer = millis() + kBlinkTime;
    update();
}

void Display::resetBlink() {
    m_blinkMin = false;
    m_blinkSec = false;
    m_blinkDots = false;
    update();
}
