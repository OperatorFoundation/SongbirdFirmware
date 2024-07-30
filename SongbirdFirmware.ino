#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

#define NOISE_MODE = 1
#define SHIFT_MODE = 0

// GUItool: begin automatically generated code
AudioInputI2S            i2sInput;           //xy=155,260
AudioMixer4              mixerL;         //xy=383,260
AudioMixer4              mixerR;         //xy=414,340

#ifdef NOISE_MODE
AudioSynthNoiseWhite     noise;         //xy=156,224
AudioConnection          patchCordInputMixerL(i2sInput, 0, mixerL, 1);
AudioConnection          patchCordInputMixerR(i2sInput, 1, mixerR, 1);

AudioConnection          patchCordNoiseMixerL(noise, 0, mixerL, 0);
AudioConnection          patchCordNoiseMixerR(noise, 0, mixerR, 0);
#endif

#ifdef SHIFT_MODE
AudioEffectGranular      pitchShiftR;      //xy=208,403
AudioEffectGranular      pitchShiftL;      //xy=230,361
AudioConnection          patchCordInputPitchShiftL(i2sInput, 0, pitchShiftL, 0);
AudioConnection          patchCordInputPitchShiftR(i2sInput, 1, pitchShiftR, 0);

AudioConnection          patchCordPitchShiftRMixerR(pitchShiftR, 0, mixerR, 2);
AudioConnection          patchCordPitchShiftLMixerL(pitchShiftL, 0, mixerL, 2);
#endif

AudioOutputUSB           usbOutput;           //xy=636,298
AudioOutputI2S           i2sOutput;           //xy=640,366

AudioConnection          patchCordUSBOutputMixerL(mixerL, 0, usbOutput, 0);
AudioConnection          patchCordUSBOutputMixerR(mixerR, 0, usbOutput, 1);

AudioConnection          patchCordI2sOutputMixerL(mixerL, 0, i2sOutput, 0);
AudioConnection          patchCordI2sOutputMixerR(mixerR, 0, i2sOutput, 1);


// GUItool: end automatically generated code

AudioControlSGTL5000     audioShield;     //xy=145,43

#define GRANULAR_MEMORY_SIZE 12800  // enough for 290 ms at 44.1 kHz
int16_t granularMemory[GRANULAR_MEMORY_SIZE];

void setup()
{
  AudioMemory(4);
  audioShield.enable();
  audioShield.inputSelect(AUDIO_INPUT_LINEIN);
  audioShield.volume(0.5);

  /// TODO: Move pitch shift code to seperate method
  // the Granular effect requires memory to operate
  pitchShiftL.begin(granularMemory, GRANULAR_MEMORY_SIZE);
  pitchShiftR.begin(granularMemory, GRANULAR_MEMORY_SIZE);
  pitchShiftL.setSpeed(4.0);
  pitchShiftR.setSpeed(4.0);
  
  Serial.println("setup done");
}

void beginModulation()
{
  #ifdef NOISE_MODE
  addNoise()
  #endif

  #ifdef SHIFT_MODE
  beginPitchShift();
  #endif
}

void loop()
{
  delay(20);
  beginModulation()
}

/// TODO: Noise tuning to High Frequency
void addNoise()
{
  // define a noise source

  // get a mixer

  // mix the input with the noise

  // output mixer results to output
}

// SINE Wave that changes its own pitch
void toneSweep()
{
  
}

void beginPitchShift()
{
  /// 1. Pitch - Pitch Shifting
  /// 2. Pitch Velocity - How fast we are changing the pitch 
  /// 3. Pitch Acceleration - How fast we are changing the way we are changing the pitch

  // Pitch shift by continuously sampling grains and playing them at altered speed. The grainLength is specified in milliseconds, up to one third of the memory from begin();

  // TODO: Tweak the sample sizes based on relevant sizes: 1 msec, 3 msec, 25 msec
  // read knobs, scale to 0-1.0 numbers
  // float knobA3 = (float)analogRead(A3) / 1023.0;
  float knobA3 = 0.25;
  float msec = 1.0 + (knobA3 * 99.0);
  pitchShiftL.beginPitchShift(msec);
  pitchShiftR.beginPitchShift(msec);
  // Serial.print("Begin granular pitch phift using ");
  // Serial.print(msec);
  // Serial.println(" grains");
}