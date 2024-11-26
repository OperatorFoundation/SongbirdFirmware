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
AudioSynthNoisePink pinkNoise; // Pink noise effect
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
AudioConnection patchCordNoiseEffectsMix(pinkNoise, 0, effectsMixer, 1); // Noise takes effects mixer slot 1

// (tone sweep -> mixer) -> USB microphone output
AudioConnection patchCordTonesweepEffectsMix(tonesweep, 0, effectsMixer, 2); // Tones sweep takes effects mixer slot 2

// Output connections: all outputs have a mixer in front of them, which is a recommended best practice for Teensy audio programming. Don't forget to set the mixer levels.

// ((Headset microphone / wav) -> pitch shift) / noise / tone sweep -> (mixer -> Headset headphones output)
AudioConnection patchCordEffectsUSB(effectsMixer, 0, outputToUSB, 0); // mixed effects output takes USB microphone output slot 0

void setupAudioProcessing()
{
  AudioMemory(12);
  audioShield.enable();
  audioShield.volume(0.5);
  audioShield.inputSelect(AUDIO_INPUT_MIC);  // AUDIO_INPUT_LINEIN or AUDIO_INPUT_MIC

  delay(1000);
  
  // Mixers

  // Output mixer for the left channel of USB headphones -> headset headphones
  leftHeadphonesMixer.gain(0, 0.5); // Left channel of USB headphones input is on slot 0
  leftHeadphonesMixer.gain(1, 0); // Empty
  leftHeadphonesMixer.gain(2, 0); // Empty
  leftHeadphonesMixer.gain(3, 0); // Empty
  
  // Output mixer for the right channel of USB headphones -> headset headphones
  rightHeadphonesMixer.gain(0, 0.5); // Right channel of USB headphones input is on slot 0
  rightHeadphonesMixer.gain(1, 0); // Empty
  rightHeadphonesMixer.gain(2, 0); // Empty
  rightHeadphonesMixer.gain(3, 0); // Empty

  setProductionDevMixer();

  // Output mixer for USB microphone audio output sourced from either headset microphone (production) or wav player (dev)
  effectsMixer.gain(0, 0.5); // Pitch shifted headset microphone (production) or wave player (dev) signal takes slot 0.
  effectsMixer.gain(1, 0.5); // Noise takes effects mixer slot 1
  effectsMixer.gain(2, 0.5); // Tones sweep takes effects mixer slot 2
}

void setProductionDevMixer()
{
  // Switches between headset microphone (production) and wav player (dev)
  if (isDevModeEnabled()) // Dev mode means all wav player, no microphone
  {
    productionDevMixer.gain(0, 0); // Headset microphone takes production/dev mixer slot 0
    productionDevMixer.gain(1, 1); // Wav player takes production/dev mixer slot 1
  }
  else // Production mode means all microphone, no wav player
  {
    productionDevMixer.gain(0, 1); // Headset microphone takes production/dev mixer slot 0
    productionDevMixer.gain(1, 0); // Wav player takes production/dev mixer slot 1    
  }
}
