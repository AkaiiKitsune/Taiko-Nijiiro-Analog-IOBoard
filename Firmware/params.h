// Select which player this board is. 1 for P1 and 2 for P2.
#define PLAYER_SELECT 1

// Enable debug mode to view analog input values from Serial.
// Make sure to use the serial plotter and not the serial monitor!
#define DEBUG 0

/**********************************************
  EVERYTHING BEYOND THIS POINT DOESN'T NEED TO
   BE EDITED EXCEPT IF YOU MODIFIED THE PCB !
***********************************************/
// ============ PIN DEFINITION ================
// Input pins.
#define L_KAT_IN 4
#define L_DON_IN 5
#define R_DON_IN 6
#define R_KAT_IN 7

// Sensitivity adjustment potentiometer input pins.
#define L_KAT_ADJUST 9
#define L_DON_ADJUST 10
#define R_DON_ADJUST 11
#define R_KAT_ADJUST 12

// LED pins.
#define L_KAT_LED 17
#define L_DON_LED 18
#define R_DON_LED 8
#define R_KAT_LED 3

// ============ OTHER SETTINGS ================
// The maximum value of a hit (not the minumum value to trigger a heavy hit)
// To configure the light and heavy thresholds, do it in the game settings.
#define MAX_THRES 5000

// LED PWM Settings.
#define ledFreq 1000
#define ledResolution 8

// Sampling Cache, a higher value means a smoother signal. 32 is a good value for the esp32.
// This value must be a power of 2 (4, 8, 16, 32, etc).
#define SAMPLE_CACHE_LENGTH 32

#define AXIS_RANGE 1023
#define PLAYERS 2
#define CHANNELS 4

#include <USB.h>
#include "joystick.h"
#include "cache.h"