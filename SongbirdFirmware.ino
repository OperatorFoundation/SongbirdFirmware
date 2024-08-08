#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

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

enum SoundMode
{
  NOISE, PITCHSHIFT, TONESWEEP, PARTY
};

SoundMode mode = PARTY;

void setup() 
{
  Serial.begin(9600);

  AudioMemory(4);
  audioShield.enable();
  audioShield.inputSelect(AUDIO_INPUT_LINEIN);
  audioShield.volume(0.5);
  
  setupPitchShift();

  startModulation();

  Serial.println("setup done");
}

void startModulation()
{
  switch (mode) 
  {
    case NOISE:
      noiseOnlyMode();
      break;
    case PITCHSHIFT:
      shiftOnlyMode();
      break;
    case TONESWEEP:
      tonesweepOnlyMode();
      break;
    case PARTY:
      partyMode();
      break;
  }
}

void loopModulation()
{
  switch (mode)
  {
    case NOISE:
      maintainNoise();
      break;
    case PITCHSHIFT:
      maintainShift();
      break;
    case TONESWEEP:
      maintainTonesweep();
      break;
    case PARTY:
      maintainParty();
      break;
  }
}

void shiftOnlyMode()
{
  stopNoise();
  startPitchShift();
}

void noiseOnlyMode()
{
  stopPitchShift();
  startNoise();
}

void tonesweepOnlyMode()
{
  stopPitchShift();
  stopNoise();
  startTonesweep();
}

void partyMode()
{
  startNoise();
  startPitchShift();
  startTonesweep();
}

void maintainParty()
{
  maintainTonesweep();
}

void loop()
{
  loopModulation();
}
