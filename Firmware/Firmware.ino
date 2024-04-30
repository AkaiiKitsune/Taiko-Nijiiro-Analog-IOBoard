/****************************************************
  IF YOU WANT TO CHANGE THE SETTINGS GO TO params.h !
*****************************************************/
#include <FastLED.h>
#include "params.h"

CRGB player_led[1];

const byte inPins[CHANNELS] = {
    L_DON_IN, L_KAT_IN, R_DON_IN, R_KAT_IN};
const byte sensitivityPins[CHANNELS] = {
    L_DON_ADJUST, L_KAT_ADJUST, R_DON_ADJUST, R_KAT_ADJUST};

const byte ledPins[CHANNELS] = {
    L_DON_LED, L_KAT_LED, R_DON_LED, R_KAT_LED};

Cache<int, SAMPLE_CACHE_LENGTH> inputWindow[CHANNELS];
unsigned long power[CHANNELS];

uint axisValues[CHANNELS] = {0, 0, 0, 0};

Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID, JOYSTICK_TYPE_GAMEPAD, 10, 4, true, true, false, true, true, false, false, false, false, false, false);

// ===================================================================================================
void setup()
{
#if DEBUG
  Serial.begin(250000);
#endif

  // Setting the internal ESP32 RGB LED to the player's color :
  // Red for player 1, Blue for player 2.
  FastLED.addLeds<NEOPIXEL, 48>(player_led, 1); // gpio48
  player_led[0] = PLAYER_SELECT == 1 ? CRGB::Red : CRGB::Aqua;
  FastLED.show();

  for (byte i = 0; i < CHANNELS; i++)
  { // Set pin mode for all input, sensitivity; and LED pins.
    pinMode(inPins[i], INPUT);
    pinMode(sensitivityPins[i], INPUT);
    pinMode(ledPins[i], OUTPUT);

    ledcSetup(i, ledFreq, ledResolution); // Init all ledc channels.
    ledcAttachPin(ledPins[i], i);         // Attach each LEDs to a different ledc pwm channel.

    // Then init the array(s) which will hold the piezzos values on each cycle.
    power[i] = 0;
  }

  // Init USB
  USB.PID(0x4869);
  USB.VID(0x4869);
  USB.productName("Nijiiro Analog IO Board");
  USB.manufacturerName("ShikyC, AkaiiKitsune");

  USB.begin();

  // Init Joystick
  Joystick.begin(false);
  Joystick.setXAxisRange(-AXIS_RANGE, AXIS_RANGE);
  Joystick.setYAxisRange(-AXIS_RANGE, AXIS_RANGE);
  Joystick.setRxAxisRange(-AXIS_RANGE, AXIS_RANGE);
  Joystick.setRyAxisRange(-AXIS_RANGE, AXIS_RANGE);
}

// ===================================================================================================
void loop()
{
  for (byte i = 0; i < CHANNELS; i++)
  {
    inputWindow[i].put(analogRead(inPins[i]));
    power[i] = power[i] - inputWindow[i].get(1) + inputWindow[i].get();

    float x = analogRead(sensitivityPins[i]) / 2048.0 - 1;
    float x2 = x * x;
    float x3 = x2 * x;
    float x4 = x3 * x;
    float v = (1.0 + x + 0.5 * x2 + 0.166667 * x3) * power[i];
    axisValues[i] = AXIS_RANGE * (v >= MAX_THRES ? 1 : (v / MAX_THRES));

#if DEBUG
    Serial.print("Channel_" + String(i) + ":");
    if (i < CHANNELS - 1)
      Serial.print(power[i] + ",");
    else
      Serial.println(power[i]);
#endif

    ledcWrite(i, (int)((axisValues[i] / AXIS_RANGE) * 255));
  }

#if PLAYER_SELECT == 1
  Joystick.setXAxis(axisValues[0] > axisValues[1] ? axisValues[0] : -axisValues[1]);
  Joystick.setYAxis(axisValues[2] > axisValues[3] ? axisValues[2] : -axisValues[3]);
  Joystick.setRxAxis(0);
  Joystick.setRyAxis(0);
#elif PLAYER_SELECT == 2
  Joystick.setXAxis(0);
  Joystick.setYAxis(0);
  Joystick.setRxAxis(axisValues[0] > axisValues[1] ? axisValues[0] : -axisValues[1]);
  Joystick.setRyAxis(axisValues[2] > axisValues[3] ? axisValues[2] : -axisValues[3]);
#else
  Joystick.setXAxis(0);
  Joystick.setYAxis(0);
  Joystick.setRxAxis(0);
  Joystick.setRyAxis(0);
#endif

  Joystick.sendState();
}
