#pragma once

#include <Config.h>

#include "Beeper.h"
#include "Display.h"
#include "Hardware.h"

// https://github.com/GyverLibs/GyverPower/blob/main/examples/lowClockTimeCorrect/lowClockTimeCorrect.ino
#define millis() (millis() << (CLKPR & 0xF))

extern Display gDisplay;
extern Hardware gHardware;
extern Beeper gBeeper;
