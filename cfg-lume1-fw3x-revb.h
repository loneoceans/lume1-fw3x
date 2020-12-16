/* lume1 for FW3x config options for Anduril
 * Constant current Buck-Boost + FET driver
 * For more information: www.loneoceans.com/labs/
 * Datasheets:
 * - 1634: http://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-8303-8-bit-AVR-Microcontroller-tinyAVR-ATtiny1634_Datasheet.pdf
 * - Manual: https://bazaar.launchpad.net/~toykeeper/flashlight-firmware/fsm/view/head:/ToyKeeper/spaghetti-monster/anduril/anduril-manual.txt
 * - Repository: https://code.launchpad.net/~toykeeper/flashlight-firmware
 */

#include "hwdef-lume1-fw3x-revb.h"
// ATTINY: 1634

// ##################################################
// ##               THERMAL CONFIG                 ##
// ##################################################

// NOTES: Anduril1 updated with Enable Pin support handling
// NOTES: Anduril1 updated to support external temp sensor handling (see HWCFG for formulas)

#define USE_THERMAL_REGULATION

// set this light for 50C thermal ceiling
#undef DEFAULT_THERM_CEIL
#define DEFAULT_THERM_CEIL 50
//#define MAX_THERM_CEIL 70 // default

// drop from turbo at therm_ceil+TURBO_TEMP_EXTRA
#define TURBO_TEMP_EXTRA 3 // in celsius

// optional, makes initial turbo step-down faster so first peak isn't as hot
// FET mode can run very very hot, so be extra careful
//#define THERM_HARD_TURBO_DROP

// stop panicking at ~85% regulated power or ~750 lm
#define THERM_FASTER_LEVEL 140

// respond to thermal changes faster
#define THERMAL_WARNING_SECONDS 3
#define THERMAL_UPDATE_SPEED 1
#define THERM_PREDICTION_STRENGTH 4
//#define THERM_RESPONSE_MAGNITUDE 128

// easier access to thermal config mode, similar to Emisar, Noctigon
#define USE_TENCLICK_THERMAL_CONFIG

#define THERM_CAL_OFFSET 0  // not needed due to external sensor

// do not recalibrate light temperature to 21C upon factory reset!
// Lume driver has a calibrated temperature sensor already
// modified main.c at line 2534
#define NO_TEMP_CAL_DURING_RESET


// ##################################################
// ##                 RAMP CONFIG                  ##
// ##################################################

// ../../bin/level_calc.py cube 1 149 7135 0 0.5 1000, with 0 appended to the end.
// for FET PWM (PWM2), all values are 0, except for last value of 1023
// (with max_pwm set to 1023)
#define RAMP_LENGTH 150
//#define PWM1_LEVELS 0,0,0,0,1,1,1,1,2,2,2,3,3,4,4,5,5,6,7,7,8,9,10,11,12,13,14,15,16,17,19,20,22,23,25,26,28,30,32,34,36,38,40,42,45,47,49,52,55,58,60,63,66,70,73,76,80,83,87,91,94,98,102,107,111,115,120,124,129,134,139,144,150,155,160,166,172,178,184,190,196,203,209,216,223,230,237,244,252,259,267,275,283,291,299,308,316,325,334,343,353,362,372,382,392,402,412,423,433,444,455,466,478,489,501,513,525,538,550,563,576,589,602,616,630,644,658,672,687,701,716,731,747,762,778,794,810,827,844,861,878,895,913,930,948,967,985,1004,1023,0
#define PWM1_LEVELS 1,1,1,1,2,2,2,2,3,3,3,4,4,5,5,6,6,7,8,8,9,10,11,12,13,14,15,16,17,18,20,21,23,24,26,27,29,31,33,35,37,39,41,43,45,48,50,53,56,58,61,64,67,70,74,77,80,84,88,91,95,99,103,108,112,116,121,125,130,135,140,145,150,156,161,167,173,178,184,191,197,203,210,217,223,230,238,245,252,260,268,275,283,292,300,308,317,326,335,344,353,363,372,382,392,402,413,423,434,445,456,467,478,490,502,514,526,538,551,563,576,589,603,616,630,644,658,672,687,702,717,732,747,763,778,794,811,827,844,861,878,895,913,931,949,967,985,1004,1023,0
#define PWM2_LEVELS 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1023

#define DEFAULT_LEVEL 56
#define MAX_1x7135 77   // approximately 500mA equivalent

//#define HALFSPEED_LEVEL 14
//#define QUARTERSPEED_LEVEL 5

// turn on BuckBoost from level 1 to 149, but not 150
// Level 150 is when BuckBoost is off and FET is ON 100%
#define LED_ENABLE_PIN_LEVEL_MIN 1
#define LED_ENABLE_PIN_LEVEL_MAX 149

// the entire ramp is regulated; don't blink halfway up
#ifdef BLINK_AT_RAMP_MIDDLE
#undef BLINK_AT_RAMP_MIDDLE
#endif

// don't slow down at low levels; this isn't that sort of light
// (it needs to stay at full speed for the 10-bit PWM to work)
#ifdef USE_DYNAMIC_UNDERCLOCKING
#undef USE_DYNAMIC_UNDERCLOCKING
#endif

#define RAMP_SMOOTH_FLOOR 1
#define RAMP_SMOOTH_CEIL 149	// Level 150 is when BuckBoost is off and FET is ON 100%
// 10, 30, [50], 70, 90, 110, 130
#define RAMP_DISCRETE_FLOOR 10
#define RAMP_DISCRETE_CEIL RAMP_SMOOTH_CEIL
#define RAMP_DISCRETE_STEPS 7

// Muggle mode: Goal from about ~10 lumens to about ~300+ lumens
// In this case, ramp number 99 is about 1A to the driver
#define MUGGLE_FLOOR RAMP_DISCRETE_FLOOR
#define MUGGLE_CEILING 99

// ##################################################
// ##              AUX & MISC CONFIG               ##
// ##################################################

// this light has three aux LED channels: R, G, B
#define USE_AUX_RGB_LEDS

// it has no independent LED in the button unlike emisar d4
//#define USE_BUTTON_LED

// the aux LEDs are front-facing, so turn them off while main LEDs are on
// TODO: the whole "indicator LED" thing needs to be refactored into
//       "aux LED(s)" and "button LED(s)" since they work a bit differently
#ifdef USE_INDICATOR_LED_WHILE_RAMPING
#undef USE_INDICATOR_LED_WHILE_RAMPING
#endif
#define RGB_LED_OFF_DEFAULT 0x18  // low, voltage
#define RGB_LED_LOCKOUT_DEFAULT 0x37  // blinking, rainbow

// enable blinking aux LEDs
#define TICK_DURING_STANDBY
#define STANDBY_TICK_SPEED 3  // every 0.128 s
//#define STANDBY_TICK_SPEED 4  // every 0.256 s
//#define STANDBY_TICK_SPEED 5  // every 0.512 s

// slow down party strobe; this driver can't pulse for too short a time
#define PARTY_STROBE_ONTIME 4

//#define THERM_CAL_OFFSET 0	// was 5 from Noctigon

// attiny1634 has enough space to smooth out voltage readings
#define USE_VOLTAGE_LOWPASS

// can't reset the normal way because power is connected before the button
#define USE_SOFT_FACTORY_RESET