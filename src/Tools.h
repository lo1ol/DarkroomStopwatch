#pragma once

#include <Config.h>

#include "Beeper.h"
#include "Display.h"
#include "Hardware.h"
#include "Settings.h"

// https://github.com/GyverLibs/GyverPower/blob/main/examples/lowClockTimeCorrect/lowClockTimeCorrect.ino
#define millis() (millis() << (CLKPR & 0xF))

#define ADD_TO_ENUM(enumName, current, num) \
    (enumName)(((uint8_t)(current) + (uint8_t)enumName::last_ + num) % (uint8_t)enumName::last_)

extern Display gDisplay;
extern Hardware gHardware;
extern Beeper gBeeper;
extern Settings gSettings;
