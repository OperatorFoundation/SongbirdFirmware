#ifndef SETTINGS_h
#define SETTINGS_h
#include <Arduino.h>
#include <Adafruit_GFX.h>       // https://learn.adafruit.com/adafruit-gfx-graphics-library/overview
#include <Adafruit_SSD1306.h>   // https://learn.adafruit.com/monochrome-oled-breakouts/wiring-128x32-i2c-display
#include <Adafruit_NeoPixel.h>  // https://learn.adafruit.com/adafruit-neopixel-uberguide/arduino-library-use
#include <Bounce2.h>            // https://www.pjrc.com/teensy/td_libs_Bounce.html

// LEDs
#define LED_1 9
#define LED_2 10

#define NEOPIXEL_PIN 22 
#define NUMPIXELS  1
Adafruit_NeoPixel pixels(NUMPIXELS, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

// Buttons
#define SW_LEFT_PIN 26
#define SW_RIGHT_PIN 29
#define SW_UP_PIN 28
#define SW_DOWN_PIN 27

const int buttonPins[] = {SW_LEFT_PIN, SW_RIGHT_PIN, SW_UP_PIN, SW_DOWN_PIN};  // Adjust pin numbers as needed
const int numButtons = sizeof(buttonPins) / sizeof(buttonPins[0]);

// Bounce leftButton = Bounce(SW_LEFT_PIN, 15);
// Bounce downButton = Bounce(SW_DOWN_PIN, 15);
// Bounce upButton = Bounce(SW_UP_PIN, 15);
// Bounce rightButton = Bounce(SW_RIGHT_PIN, 15);

// uint8_t leftButtonOff;
// uint8_t downButtonOff;
// uint8_t upButtonOff;
// uint8_t rightButtonOff;

// Display
#define SCREEN_WIDTH 128    // OLED display width, in p ixels
#define SCREEN_HEIGHT 32    // OLED display height, in pixels
#define SCREEN_ADDRESS 0x3C // 0x3C for 128x32
#define OLED_RESET     -1   // Reset pin #, if not connected use -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire1, OLED_RESET);  // display is connected to second I2C


#define I2S_DIN 7
#define I2S_DOUT 8
#define SYS_MCLK 23
#define I2S_SCLK 21
#define I2S_LRCLK 20


#define HPAMP_VOL_CLK 38
#define HPAMP_VOL_UD 37
#define HPAMP_SHUTDOWN 36

// AUDIO
AudioSynthNoiseWhite     noise;         //xy=62,259
AudioInputI2S            i2sInput;           //xy=62,368
AudioSynthToneSweep      tonesweep;     //xy=69,313
AudioEffectGranular      lPitchShift;      //xy=254,351
AudioEffectGranular      rPitchShift;      //xy=254,388
AudioMixer4              lMixer;         //xy=462,271
AudioMixer4              rMixer;         //xy=462,345
AudioOutputUSB           usbOutput;           //xy=685,277
AudioOutputI2S           i2sOutput;           //xy=686,338
AudioConnection          noiseToLMixer(noise, 0, lMixer, 0);
AudioConnection          noiseToRMixer(noise, 0, rMixer, 0);
AudioConnection          i2sInputToLShifter(i2sInput, 0, lPitchShift, 0);
AudioConnection          i2sInputToRShifter(i2sInput, 1, rPitchShift, 0);
AudioConnection          tonesweepToLMixer(tonesweep, 0, lMixer, 1);
AudioConnection          tonesweepToRMixer(tonesweep, 0, rMixer, 1);
AudioConnection          lShifterToLMixer(lPitchShift, 0, lMixer, 2);
AudioConnection          rShifterToRMixer(rPitchShift, 0, rMixer, 2);
AudioConnection          lMixerToUSBOut(lMixer, 0, usbOutput, 0);
AudioConnection          lMixerToI2SOut(lMixer, 0, i2sOutput, 0);
AudioConnection          rMixerToUSBOut(rMixer, 0, usbOutput, 1);
AudioConnection          rMixerToI2SOut(rMixer, 0, i2sOutput, 1);
AudioControlSGTL5000     audioShield;     //xy=145,43

#endif