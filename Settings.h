#ifndef _SETTINGS_H_
#define _SETTINGS_H_

#include <Arduino.h>
#include <Adafruit_GFX.h>       // https://learn.adafruit.com/adafruit-gfx-graphics-library/overview
#include <Adafruit_SSD1306.h>   // https://learn.adafruit.com/monochrome-oled-breakouts/wiring-128x32-i2c-display
#include <Adafruit_NeoPixel.h>  // https://learn.adafruit.com/adafruit-neopixel-uberguide/arduino-library-use
#include <Bounce2.h>            // https://www.pjrc.com/teensy/td_libs_Bounce.html
//#include <WaveFile.h>

#include "mixer.h"
#include "AudioStream.h"
#include "usb_audio.h"

// LEDs
#define LED_1 9
#define LED_2 10

#define NEOPIXEL_PIN 22 
#define NUMPIXELS  1
Adafruit_NeoPixel pixels(NUMPIXELS, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

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

// Codecs
AudioControlSGTL5000 audioShield;

// Inputs
AudioInputI2S inputFromHeadset; // Headset microphone input (production)
AudioInputUSB inputFromUSB; // USB headphones input

// Generators
AudioPlaySdWav player; // Wav player input (dev)
AudioSynthNoiseWhite noise; // White noise effect
AudioSynthToneSweep tonesweep; // Tonesweep effect

// Effects
AudioEffectGranular pitchShifter; // Pitch shift effect

// Mixers
AudioMixer4 leftHeadphonesMixer; // Output mixer for left channel of headset headphones
AudioMixer4 rightHeadphonesMixer; // Output mixer for right channel of headset headphones
AudioMixer4 productionDevMixer; // Input mixer to switch between production and dev audio signals
AudioMixer4 effectsMixer; // Output mixer for USB microphone audio output sourced from either headset microphone (production) or wav player (dev)

// Outputs
AudioOutputI2S outputToHeadset; // Headset headphones output
AudioOutputUSB outputToUSB; // USB microphone output

// Connections

// Headset -> Phone:
// USB headphones input -> mixer -> Headset headphones output

// (USB headphones input -> mixer) -> Headset headphones output. Separate mixers for left and right channels so that we keep the stereo signals separate.
AudioConnection patchCordUSBLeftMixer(inputFromUSB, 0, leftHeadphonesMixer, 0); // USB headphones input left channel takes left headphones mixer slot 0
AudioConnection patchCordUSBRightMixer(inputFromUSB, 1, rightHeadphonesMixer, 0); // USB headphones input right channel takes right headphones mixer slot 1

// USB headphones input -> (mixer -> Headset headphones output). Separate mixers for left and right channels so that we keep the stereo signals separate.
AudioConnection patchCordLeftMixerHeadset(leftHeadphonesMixer, 0, outputToHeadset, 0); // left headphone channel takes headset headphones output slot 0
AudioConnection patchCordRightMixerHeadset(rightHeadphonesMixer, 0, outputToHeadset, 1); // right headphone channel takes headset headphones output slot 1

// Phone/SD -> Headset:
// Headset microphone input -> effects -> mixer -> USB microphone output  (production mode)
// Wav file player input -> effects -> mixer -> USB microphone output (DEV_MODE)

// Input connections: Headset microphone input / Wave file player input -> mixer; USB headphones input -> mixer.

// (Headset microphone input -> mixer) -> effects -> mixer -> USB microphone output  (production mode)
AudioConnection patchCordHeadsetPDMix(inputFromHeadset, 0, productionDevMixer, 0); // Headset microphone takes production/dev mixer slot 0

// (Wav file player input -> mixer) -> effects -> mixer -> USB microphone output (DEV_MODE)
AudioConnection patchCordWavPDMix(player, 0, productionDevMixer, 1); // Wav player takes production/dev mixer slot 1

// Effect connections - all effects are on the Headset microphone input -> USB headphones output signal path.

// Effects to mixers: noise -> mixer; pitch shifter -> mixer (production); pitch shifter -> mixer (dev); tone sweep -> mixer.

// Headset / wav input -> (mixer -> pitch shift) -> mixer -> USB microphone output
AudioConnection patchCordPDPitchshift(productionDevMixer, 0, pitchShifter, 0); // production/dev mixer takes pitchShifter slot 0

// Headset / wav input -> mixer -> (pitch shift -> mixer) -> USB microphone output
AudioConnection patchCordPitchshiftEffectsMix(pitchShifter, 0, effectsMixer, 0); // Pitchshifted Headest / wav input takes effects mixer slot 0

// (noise -> mixer) -> USB microphone output
AudioConnection patchCordNoiseEffectsMix(noise, 0, effectsMixer, 1); // Noise takes effects mixer slot 1

// (tone sweep -> mixer) -> USB microphone output
AudioConnection patchCordTonesweepEffectsMix(tonesweep, 0, effectsMixer, 2); // Tones sweep takes effects mixer slot 2

// Output connections: all outputs have a mixer in front of them, which is a recommended best practice for Teensy audio programming. Don't forget to set the mixer levels.

// ((Headset microphone / wav) -> pitch shift) / noise / tone sweep -> (mixer -> Headset headphones output)
AudioConnection patchCordEffectsUSB(effectsMixer, 0, outputToUSB, 0); // mixed effects output takes USB microphone output slot 0

#endif
