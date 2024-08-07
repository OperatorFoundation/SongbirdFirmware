#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

#define GRANULAR_MEMORY_SIZE 12800  // enough for 290 ms at 44.1 kHz
int16_t                  granularMemory[GRANULAR_MEMORY_SIZE];


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

void setupPitchShift()
{
  // the Granular effect requires memory to operate
  lPitchShift.begin(granularMemory, GRANULAR_MEMORY_SIZE);
  rPitchShift.begin(granularMemory, GRANULAR_MEMORY_SIZE);
  lPitchShift.setSpeed(4.0);
  rPitchShift.setSpeed(4.0);
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
      break;
    case TONESWEEP:
      maintainTonesweep();
      break;
    case PARTY:
      maintainParty();
      break;
  }
}

void startPitchShift()
{
  /// 1. Pitch - Pitch Shifting
  /// 2. Pitch Velocity - How fast we are changing the pitch 
  /// 3. Pitch Acceleration - How fast we are changing the way we are changing the pitch

  // Pitch shift by continuously sampling grains and playing them at altered speed. The grainLength is specified in milliseconds, up to one third of the memory from begin();

  // TODO: Tweak the sample sizes based on relevant sizes: 1 msec, 3 msec, 25 msec
  float someValue = 0.25; // 
  float msec = 1.0 + (someValue * 99.0);
  lPitchShift.beginPitchShift(msec);
  rPitchShift.beginPitchShift(msec);

  // TODO: We may want to do this based on some algorithmic value, the example is based on user input
  // // Continuously adjust the speed, based on the A3 pot
  // float ratio;
  // ratio = powf(2.0, knobA2 * 2.0 - 1.0); // 0.5 to 2.0
  // //ratio = powf(2.0, knobA2 * 6.0 - 3.0); // 0.125 to 8.0 -- uncomment for far too much range!
  // granular1.setSpeed(ratio);
}

void stopPitchShift()
{
  lPitchShift.stop();
  rPitchShift.stop();
}



/// Create a continuously varying (in frequency) sine wave
/// Note: Uses excessive CPU time.
/// The frequency actually changes in discrete steps every 128 samples (2.9 ms).
void startTonesweep()
{
  // specifies the amplitude,
  // start and end frequencies (which can sweep up or down)
  // and the length of time of the sweep.
  float sweepAmplitude = 0.8;
  int sweepLo = 10;
  int sweepHi = 22000;
  float sweepTime = 5; // Length of time for the sweep in seconds

  // play(level, lowFreq, highFreq, time);
  // Start generating frequency sweep output. The time is specified in seconds. Level is 0 to 1.0
  tonesweep.play(sweepAmplitude, sweepLo, sweepHi, sweepTime);
}

void maintainTonesweep()
{
  if (!tonesweep.isPlaying())
  {
    startTonesweep();
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
