#include "Tools.h"

Display gDisplay;
Hardware gHardware;
Beeper gBeeper;
Settings gSettings;

// https://github.com/GyverLibs/GyverPower/blob/main/examples/lowClockTimeCorrect/lowClockTimeCorrect.ino
uint32_t gMillis() {
    return millis() << (CLKPR & 0xF);
}

