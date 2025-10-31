#pragma once

// Use user config values if exists
#if __has_include("../UserConfig.h")
    #include "../UserConfig.h"
#endif

// WARNING: Don't change values here
// Create ../UserConfig.h file to redefine values from here

#include <assert.h>

#include <Arduino.h>

#ifndef ENCODER_DT_PIN
    #define ENCODER_DT_PIN 2
#endif

#ifndef ENCODER_CLK_PIN
    #define ENCODER_CLK_PIN 3
#endif

#if ENCODER_DT_PIN == 2 && ENCODER_CLK_PIN == 3
    #define ENCODER_DIRECTION (1)
#elif ENCODER_DT_PIN == 3 && ENCODER_CLK_PIN == 2
    #define ENCODER_DIRECTION (-1)
#else
static_assert(false && "Bad values for ENCODER_DT_PIN and ENCODER_CLK_PIN");
#endif

#ifndef START_BTN_PIN
    #define START_BTN_PIN 4
#endif

#ifndef RESET_BTN_PIN
    #define RESET_BTN_PIN 5
#endif

#ifndef DISPLAY_CLK_PIN
    #define DISPLAY_CLK_PIN 6
#endif

#ifndef DISPLAY_DIO_PIN
    #define DISPLAY_DIO_PIN 7
#endif

#ifndef ENCODER_PWR_PIN
    #define ENCODER_PWR_PIN A1
#endif

#ifndef DISPLAY_PWR_PIN
    #define DISPLAY_PWR_PIN A2
#endif

#ifndef BEEPER_PIN
    #define BEEPER_PIN 9
#endif

#ifndef MS_IN_SEC
    #define MS_IN_SEC 997L
#endif

#ifndef AUTOSLEEP_TIME_SEC
    #define AUTOSLEEP_TIME_SEC (5 * 60)
#endif

#ifndef DISPLAY_BRIGHTNESS
    #define DISPLAY_BRIGHTNESS 0
#endif
static_assert(0 <= DISPLAY_BRIGHTNESS && DISPLAY_BRIGHTNESS <= 7, "Invalid value for DISPLAY_BRIGHTNESS");

#ifndef BUZZER_VOLUME
    #define BUZZER_VOLUME 255
#endif
static_assert(0 <= BUZZER_VOLUME && BUZZER_VOLUME <= 255, "Invalid value for BUZZER_VOLUME");
