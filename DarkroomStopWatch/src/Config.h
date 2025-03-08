#pragma once

#define ENCODER_DT 2
#define ENCODER_CLK 3
#define START_BTN 4
#define RESET_BTN 5
#define DISPLAY_CLK 6
#define DISPLAY_DIO 7
#define ENCODER_PWR A1
#define DISPLAY_PWR A2
#define BEEPER 9

// you timer could be behind or hurry
// you can adjust it using this param
#define MS_IN_SEC 997L
#define AUTOSLEEP_TIME_SEC 5 * 60
// 0 -- min, 7 -- max
#define DISPLAY_BRIGHTNESS 0
// 0 --min  255 --max
#define BUZZER_VOLUME 255

