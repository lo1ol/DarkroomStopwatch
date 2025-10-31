// This file could be used to change configuration of project

// To change the configuration create a copy of this file
// and name it 'UserConfig.h'

// Leave there only that values that you want to change

/* Encoder rotary pins
 *
 * WARNING: You shouldn't change them,
 * otherwise encoder could stop work properly.
 * You allowed only swap them, to change encoder direction
 */
#define ENCODER_DT_PIN 2
#define ENCODER_CLK_PIN 3

/* Pin values of hardware
 */
#define START_BTN_PIN 4
#define RESET_BTN_PIN 5
#define DISPLAY_CLK_PIN 6
#define DISPLAY_DIO_PIN 7
#define ENCODER_PWR_PIN A1
#define DISPLAY_PWR_PIN A2
#define BEEPER_PIN 9

/* You timer could be behind or hurry
 * You can adjust this value to calibrate it
 */
#define MS_IN_SEC 997L

/* Number of seconds after which timer goes to sleep mode
 */
#define AUTOSLEEP_TIME_SEC (5 * 60)

/* Display brightness
 * Value should be between 0 and 7
 */
#define DISPLAY_BRIGHTNESS 0

/* Buzzer volume
 * Value should be between 0 and 255
 */
#define BUZZER_VOLUME 255
