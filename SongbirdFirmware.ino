#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioInputUSB            usbIn;           //xy=153,87
AudioInputI2S            i2sIn;           //xy=155,126
AudioEffectGranular      granularEffect;      //xy=384,121
AudioOutputI2S           i2sOut;           //xy=627,88
AudioOutputUSB           usbOut;           //xy=628,127
AudioConnection          patchCord1(usbIn, 0, i2sOut, 0);
AudioConnection          patchCord2(usbIn, 1, i2sOut, 1);
AudioConnection          patchCord3(i2sIn, 0, granularEffect, 0);
AudioConnection          patchCord4(granularEffect, 0, usbOut, 0);
AudioControlSGTL5000     audioShield;     //xy=145,43
// GUItool: end automatically generated code


#define GRANULAR_MEMORY_SIZE 12800  // enough for 290 ms at 44.1 kHz
int16_t granularMemory[GRANULAR_MEMORY_SIZE];

void setup()
{
  AudioMemory(4);
  audioShield.enable();
  audioShield.inputSelect(AUDIO_INPUT_LINEIN);
  audioShield.volume(0.5);

  // the Granular effect requires memory to operate
  granularEffect.begin(granularMemory, GRANULAR_MEMORY_SIZE);
  granularEffect.setSpeed(4.0);
  
  Serial.println("setup done");
}

void loop()
{
  delay(20);
  beginPitchShift();
}

void beginPitchShift()
{
  // Pitch shift by continuously sampling grains and playing them at altered speed. The grainLength is specified in milliseconds, up to one third of the memory from begin();

  // read knobs, scale to 0-1.0 numbers
  // float knobA3 = (float)analogRead(A3) / 1023.0;
  float knobA3 = 0.25;
  float msec = 25.0 + (knobA3 * 75.0);
  granularEffect.beginPitchShift(msec);
  // Serial.print("Begin granular pitch phift using ");
  // Serial.print(msec);
  // Serial.println(" grains");
}