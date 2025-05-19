#define I2S_DIN 7
#define I2S_DOUT 8
#define SYS_MCLK 23
#define I2S_SCLK 21
#define I2S_LRCLK 20

#define HPAMP_VOL_CLK 0
#define HPAMP_VOL_UD 1
#define HPAMP_SHUTDOWN 2

// OpenAudio constants
float gain_dB = -15.0f;
float gain = 0.177828f;  // Same as -15 dB
float sign = 1.0f;
float deltaGain_dB = 2.5f;
// float frequencyLO = 100.0f; 
float frequencyLO = -50.0f; 
float delayms = 1.0f;

// AUDIO

// Codecs
AudioControlSGTL5000 audioShield;

// Inputs
AudioInputI2S inputFromHeadset; // Headset microphone input (production)
AudioInputUSB inputFromUSB; // USB headphones input

// Generators
AudioPlaySdWav player; // Wav player input (dev)
// AudioSynthNoiseBrown brownNoise;
AudioSynthNoisePink pinkNoise; // Pink noise effect
AudioSynthWaveformSine waveform; // Sine wave for Waveform effect

// Audio to OpenAudio converters
AudioConvert_I16toF32 itof; 

// Mixers
AudioMixer4 leftHeadphonesMixer; // Output mixer for left channel of headset headphones
AudioMixer4 rightHeadphonesMixer; // Output mixer for right channel of headset headphones
AudioMixer4 productionDevMixer; // Input mixer to switch between production and dev audio signals
AudioMixer4 effectsMixer; // Output mixer for USB microphone audio output sourced from either headset microphone (production) or wav player (dev)

// Mixers: OpenAudio
RadioIQMixer_F32 iqmixer;
AudioMixer4_F32 sum; // Summing node for the SSB receiver

// OpenAudio to Audio converters
AudioConvert_F32toI16 ftoi;

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

// (USB Audio Input -> Mixer) -> effects -> mixer -> USB microphone output (DEV_MODE) * stereo (2 channels)
AudioConnection patchCordUSBPDMix1(inputFromUSB, 0, productionDevMixer, 2);
AudioConnection patchCordUSBPDMix2(inputFromUSB, 1, productionDevMixer, 3);

// Effect connections - all effects are on the Headset microphone input -> USB headphones output signal path.

// Effects to mixers: noise -> mixer; pitch shifter -> mixer (production); pitch shifter -> mixer (dev); waveform -> mixer.

// Headset / wav input -> (mixer -> pitch shift) -> mixer -> USB microphone output
// production/dev mixer takes pitchShifter slot 0
// AudioConnection patchCordPDPitchshift(productionDevMixer, 0, pitchShifter, 0); 
AudioConnection patchCordProductionDevToConverter(productionDevMixer, 0, itof, 0); // connect to Left codec, 16-bit
AudioConnection_F32 patchCordConverterToIQL(itof, 0, iqmixer, 0); // Input to 2 mixer channels
AudioConnection_F32 patchCordConverterToIQR(itof, 0, iqmixer, 1);
AudioConnection_F32 patchCordIQToSumL(iqmixer, 0, sum, 0); // Broadband 90 deg phase
AudioConnection_F32 patchCordIQToSumR(iqmixer, 1, sum, 1);
AudioConnection_F32 patchCordSumToConverter(sum, 0, ftoi, 0); // connect to the left output

// Headset / wav input -> mixer -> (pitch shift -> mixer) -> USB microphone output
// AudioConnection patchCordPitchshiftEffectsMix(pitchShifter, 0, effectsMixer, 0); // Pitchshifted Headest / wav input takes effects mixer slot 0
AudioConnection patchCordConverterToEffectsMixer(ftoi, 0, effectsMixer, 0);
AudioConnection patchCordPassthrough(productionDevMixer, 0, effectsMixer, 3);

// (noise -> mixer) -> USB microphone output
AudioConnection patchCordNoiseEffectsMix(pinkNoise, 0, effectsMixer, 1); // Noise takes effects mixer slot 1

// (Waveform -> mixer) -> USB microphone output
AudioConnection patchCordWaveformEffectsMix(waveform, 0, effectsMixer, 2); // Sine wave for waveform takes effects mixer slot 2

// Output connections: all outputs have a mixer in front of them, which is a recommended best practice for Teensy audio programming. Don't forget to set the mixer levels.

// ((Headset microphone / wav) -> pitch shift) / noise / waveform -> (mixer -> Headset headphones output)
AudioConnection patchCordEffectsUSB(effectsMixer, 0, outputToUSB, 0); // mixed effects output takes USB microphone output slot 0

void setupAudioProcessing()
{
  AudioMemory(12);
  audioShield.enable();
  audioShield.volume(0.5);
  audioShield.inputSelect(AUDIO_INPUT_MIC);  // AUDIO_INPUT_LINEIN or AUDIO_INPUT_MIC
  audioShield.adcHighPassFilterEnable();

  // OpenAudio constants
  const float sample_rate_Hz = 44117.f;
  const int audio_block_samples = 128;
  AudioSettings_F32 audio_settings(sample_rate_Hz, audio_block_samples);
  AudioMemory_F32(200, audio_settings);  

  delay(1000);

  // Setup Headphones
  digitalWrite(HPAMP_VOL_CLK,  LOW);
  digitalWrite(HPAMP_VOL_UD,   LOW);
  digitalWrite(HPAMP_SHUTDOWN, LOW); // LOW to enable headphone amp
  pinMode(HPAMP_VOL_CLK,  OUTPUT);
  pinMode(HPAMP_VOL_UD,   OUTPUT);
  pinMode(HPAMP_SHUTDOWN, OUTPUT);
  
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
  effectsMixer.gain(0, 0.0); // Pitch shifted headset microphone (production) or wave player (dev) signal takes slot 0.
  effectsMixer.gain(1, 0.5); // Noise takes effects mixer slot 1
  effectsMixer.gain(2, 0.5); // Waveform takes effects mixer slot 2
  effectsMixer.gain(3, 0.5); // Pass-through (non-pitch-shifted) audio takes slot 3

  iqmixer.setGainOut(0.5);

  sum.gain(0, 0.5);
  sum.gain(1, 0.5);
}

void setProductionDevMixer()
{
  // Switches between headset microphone (production) and wav player (dev)
  if (isDevModeEnabled()) // Dev mode means all wav player, no microphone
  {
    productionDevMixer.gain(0, 0); // Headset microphone takes production/dev mixer slot 0
    // productionDevMixer.gain(1, 1); // Wav player (SD Card) takes production/dev mixer slot 1
    productionDevMixer.gain(2, 1); // USB Audio channel 1 takes production/dev mixer slot 2
    productionDevMixer.gain(3, 1); // USB Audio channel 2 takes production/dev mixer slot 3
  }
  else // Production mode means all microphone, no wav player
  {
    productionDevMixer.gain(0, 1); // Headset microphone takes production/dev mixer slot 0
    productionDevMixer.gain(1, 0); // Wav player takes production/dev mixer slot 1    
    productionDevMixer.gain(2, 0); // USB Audio channel 1 takes production/dev mixer slot 2
    productionDevMixer.gain(3, 0); // USB Audio channel 2 takes production/dev mixer slot 3
  }
}
