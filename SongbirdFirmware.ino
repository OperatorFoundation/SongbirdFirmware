#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

#define NOISE_MODE = 1
#define SHIFT_MODE = 0

// GUItool: begin automatically generated code
AudioInputI2S            i2sInput;
AudioMixer4              mixerL;
AudioMixer4              mixerR;

#ifdef NOISE_MODE
AudioSynthNoiseWhite     noise;
float                    noiseAmplitude = 0.25;
AudioConnection          connI2sInputToLMixer(i2sInput, 0, mixerL, 1);
AudioConnection          connI2sInputToRMixer(i2sInput, 1, mixerR, 1);
AudioConnection          connNoiseToLMixer(noise, 0, mixerL, 0);
AudioConnection          connNoiseToRMixer(noise, 0, mixerR, 0);
#endif

#ifdef SHIFT_MODE 
AudioEffectGranular      pitchShiftR;
AudioEffectGranular      pitchShiftL;
AudioConnection          connI2sInputToLPitchShift(i2sInput, 0, pitchShiftL, 0);
AudioConnection          connI2sInputRPitchShift(i2sInput, 1, pitchShiftR, 0);
AudioConnection          connLPitchShiftToLMixer(pitchShiftL, 0, mixerL, 2);
AudioConnection          connRPitchShiftToRMixer(pitchShiftR, 0, mixerR, 2);
#endif

AudioOutputUSB           usbOutput;
AudioOutputI2S           i2sOutput;

AudioConnection          connLMixerToUSBOutput(mixerL, 0, usbOutput, 0);
AudioConnection          connRMixerToUSBOutput(mixerR, 0, usbOutput, 1);
AudioConnection          connLMixerToI2sOutput(mixerL, 0, i2sOutput, 0);
AudioConnection          connRMixerToI2sOutput(mixerR, 0, i2sOutput, 1);

AudioControlSGTL5000     audioShield;     //xy=145,43

#define GRANULAR_MEMORY_SIZE 12800  // enough for 290 ms at 44.1 kHz
int16_t granularMemory[GRANULAR_MEMORY_SIZE];

void setup()
{
  AudioMemory(4);
  audioShield.enable();
  audioShield.inputSelect(AUDIO_INPUT_LINEIN);
  audioShield.volume(0.5);

  #ifdef NOISE_MODE
  setupNoise();
  #endif

  #ifdef SHIFT_MODE
  setupPitchShift();
  #endif
  
  Serial.println("setup done");
}

void setupNoise()
{
  // Set the output peak level, from 0 (off) to 1.0. The default is off. Noise is generated only after setting to a non-zero level.
  noise.amplitude(noiseAmplitude)
}

void setupPitchShift()
{
  // the Granular effect requires memory to operate
  pitchShiftL.begin(granularMemory, GRANULAR_MEMORY_SIZE);
  pitchShiftR.begin(granularMemory, GRANULAR_MEMORY_SIZE);
  pitchShiftL.setSpeed(4.0);
  pitchShiftR.setSpeed(4.0);
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
  // get a mixer

  // mix the input with the noise

  // output mixer results to output
}

// TODO: SINE Wave that changes its own pitch
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