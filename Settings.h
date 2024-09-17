#include "mixer.h"
#include "AudioStream.h"
#include "usb_audio.h"
#ifndef SETTINGS_h
#define SETTINGS_h
#include <Arduino.h>
#include <Adafruit_GFX.h>       // https://learn.adafruit.com/adafruit-gfx-graphics-library/overview
#include <Adafruit_SSD1306.h>   // https://learn.adafruit.com/monochrome-oled-breakouts/wiring-128x32-i2c-display
#include <Adafruit_NeoPixel.h>  // https://learn.adafruit.com/adafruit-neopixel-uberguide/arduino-library-use
#include <Bounce2.h>            // https://www.pjrc.com/teensy/td_libs_Bounce.html
#include <WaveFile.h>

// Use these with the Teensy 3.5 & 3.6 & 4.1 SD card
#define SDCARD_CS_PIN    BUILTIN_SDCARD
File audioFile;

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

const int buttonPins[] = {SW_LEFT_PIN, SW_RIGHT_PIN, SW_UP_PIN, SW_DOWN_PIN};
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

// GUItool: begin automatically generated code
// AudioInputI2S            inputFromHeadset;           //xy=111.5,264
// AudioInputUSB            inputFromUSB;           //xy=212,608
// AudioSynthNoiseWhite     noise;         //xy=403,345
// AudioEffectGranular      granularLeft;      //xy=413,241
// AudioEffectGranular      granularRight;      //xy=414,289
// AudioSynthToneSweep      tonesweep;     //xy=414,398
// AudioMixer4              leftHeadsetMixer;         //xy=559,574
// AudioMixer4              rightHeadsetMixer;         //xy=559,660
// AudioMixer4              leftUSBMixer;         //xy=798,263
// AudioMixer4              rightUSBMixer;         //xy=799,341
// AudioOutputI2S           outputToHeadset;           //xy=878,623
// AudioOutputUSB           outputToUSB;           //xy=1088,307

// AudioConnection          patchCord1(inputFromHeadset, 0, granularLeft, 0);
// AudioConnection          patchCord2(inputFromHeadset, 1, granularRight, 0);

// AudioConnection          patchCord3(inputFromUSB, 0, leftHeadsetMixer, 0);
// AudioConnection          patchCord4(inputFromUSB, 1, rightHeadsetMixer, 0);

// AudioConnection          patchCord5(noise, 0, leftUSBMixer, 2);
// AudioConnection          patchCord6(noise, 0, rightUSBMixer, 2);

// AudioConnection          patchCord7(granularLeft, 0, leftUSBMixer, 1);
// AudioConnection          patchCord8(granularRight, 0, rightUSBMixer, 1);

// AudioConnection          patchCord9(tonesweep, 0, leftUSBMixer, 3);
// AudioConnection          patchCord10(tonesweep, 0, rightUSBMixer, 3);

// AudioConnection          patchCord11(leftHeadsetMixer, 0, outputToHeadset, 0);
// AudioConnection          patchCord12(rightHeadsetMixer, 0, outputToHeadset, 1);

// AudioConnection          patchCord13(leftUSBMixer, 0, outputToUSB, 0);
// AudioConnection          patchCord14(rightUSBMixer, 0, outputToUSB, 1);
// GUItool: end automatically generated code

// GUItool: begin automatically generated code
AudioInputI2S            inputFromHeadset;           //xy=111.5,264
AudioInputUSB            inputFromUSB;           //xy=115,595
AudioRecordQueue         audioQueue;  // Queue for recording audio
AudioSynthNoiseWhite     noise;         //xy=407,325
AudioEffectGranular      granularLeft;      //xy=412,252
AudioEffectGranular      granularRight;      //xy=414,289
AudioSynthToneSweep      tonesweep;     //xy=417,361
AudioEffectGranular      devGranularLeft;      //xy=546,460
AudioEffectGranular      devGranularRight;      //xy=549,505
AudioMixer4              leftHeadsetMixer;         //xy=559,574
AudioMixer4              rightHeadsetMixer;         //xy=559,660
AudioMixer4              leftUSBMixer;         //xy=798,263
AudioMixer4              rightUSBMixer;         //xy=799,341
AudioOutputI2S           outputToHeadset;           //xy=878,623
AudioOutputUSB           outputToUSB;           //xy=1088,307

// Headset to effect
AudioConnection          patchCord1(inputFromHeadset, 0, granularLeft, 0);
AudioConnection          patchCord2(inputFromHeadset, 1, granularRight, 0);

// USB to headset mixer
AudioConnection          patchCord3(inputFromUSB, 0, leftHeadsetMixer, 0);
AudioConnection          patchCord5(inputFromUSB, 1, rightHeadsetMixer, 0);

// USB to effect (DEV_MODE)
AudioConnection          patchCord4(inputFromUSB, 0, devGranularLeft, 0);
AudioConnection          patchCord6(inputFromUSB, 1, devGranularRight, 0);

// Effects to USB mixer
AudioConnection          patchCord7(noise, 0, leftUSBMixer, 2);
AudioConnection          patchCord8(noise, 0, rightUSBMixer, 2);
AudioConnection          patchCord9(granularLeft, 0, leftUSBMixer, 1);
AudioConnection          patchCord10(granularRight, 0, rightUSBMixer, 1);
AudioConnection          patchCord11(tonesweep, 0, leftUSBMixer, 3);
AudioConnection          patchCord12(tonesweep, 0, rightUSBMixer, 3);
AudioConnection          patchCord13(devGranularLeft, 0, leftUSBMixer, 0);
AudioConnection          patchCord14(devGranularRight, 0, rightUSBMixer, 0);

// Headset mixer to headset
AudioConnection          patchCord15(leftHeadsetMixer, 0, outputToHeadset, 0);
AudioConnection          patchCord16(rightHeadsetMixer, 0, outputToHeadset, 1);

// USB mixer to USB
AudioConnection          patchCord17(leftUSBMixer, 0, outputToUSB, 0);
AudioConnection          patchCord18(rightUSBMixer, 0, outputToUSB, 1);

// USB Mixer to AudioQueue (SD card recording)
AudioConnection          patchCord19(leftUSBMixer, 0, audioQueue, 0);
AudioConnection          patchCord20(rightUSBMixer, 0, audioQueue, 1);

// GUItool: end automatically generated code

// AudioSynthNoiseWhite     noise;         //xy=62,259
// AudioInputUSB            usbInput;
// AudioInputI2S            i2sInput;           //xy=62,368
// AudioSynthToneSweep      tonesweep;     //xy=69,313
// AudioEffectGranular      lPitchShift;      //xy=254,351
// AudioEffectGranular      rPitchShift;      //xy=254,388
// AudioMixer4              lUSBMixer;         //xy=462,271
// AudioMixer4              rUSBMixer;         //xy=462,345
// AudioMixer4              lHeadsetMixer;         //xy=462,271
// AudioMixer4              rHeadsetMixer;         //xy=462,345             
// AudioOutputUSB           usbOutput;           //xy=685,277
// AudioOutputI2S           i2sOutput;           //xy=686,338

// // Input from Headset to pitch shifter effect
// AudioConnection          headsetInputToLShifter(i2sInput, 0, lPitchShift, 0);
// AudioConnection          headsetInputToRShifter(i2sInput, 1, rPitchShift, 0);

// // Input from Headset to USB Mixer
// AudioConnection          headsetInputToLUSBMixer();
// AudioConnection          headsetInputToRUSBMixer();

// // Input from USB to Headset Mixer
// AudioConnection          usbInputToLHeadsetMixer(usbInput, 0, lHeadsetMixer, 0);
// AudioConnection          usbInputToRHeadsetMixer(usbInput, 1, rHeadsetMixer, 0);

// // Effects to Mixers

// // Noise Effect to USB Mixer
// AudioConnection          noiseToLUSBMixer(noise, 0, lUSBMixer, 0);
// AudioConnection          noiseToRUSBMixer(noise, 0, rUSBMixer, 0);

// // Tonesweep Effect to USB Mixer
// AudioConnection          tonesweepToLUSBMixer(tonesweep, 0, lUSBMixer, 1);
// AudioConnection          tonesweepToRUSBMixer(tonesweep, 0, rUSBMixer, 1);

// // Pitch Shift Effect to USB Mixer
// AudioConnection          lShifterToLUSBMixer(lPitchShift, 0, lUSBMixer, 2);
// AudioConnection          rShifterToUSBRMixer(rPitchShift, 0, rUSBMixer, 2);

// // Mixers to Output

// // USB Mixers to USB Out
// AudioConnection          lUSBMixerToUSBOut(lUSBMixer, 0, usbOutput, 0);
// AudioConnection          rUSBMixerToUSBOut(rUSBMixer, 0, usbOutput, 1);

// // HeadsetMixer to Headset Output
// AudioConnection          lHeadsetMixerToHeadsetOut(lHeadsetMixer, 0, i2sOutput, 0);
// AudioConnection          rHeadsetMixerToHeadsetOut(rHeadsetMixer, 0, i2sOutput, 1);

AudioControlSGTL5000     audioShield;     //xy=145,43

#endif