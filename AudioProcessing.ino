#include "config.h"

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

  // Switches between headset microphone (production) and wav player (dev)
  #ifdef DEV_MODE // Dev mode means all wav player, no microphone
  productionDevMixer.gain(0, 0); // Headset microphone takes production/dev mixer slot 0
  productionDevMixer.gain(1, 1); // Wav player takes production/dev mixer slot 1
  #else // Production mode means all microphone, no wav player
  productionDevMixer.gain(0, 1); // Headset microphone takes production/dev mixer slot 0
  productionDevMixer.gain(1, 0); // Wav player takes production/dev mixer slot 1
  #endif

  // Output mixer for USB microphone audio output sourced from either headset microphone (production) or wav player (dev)
  effectsMixer.gain(0, 0.5); // Pitch shifted headset microphone (production) or wave player (dev) signal takes slot 0.
  effectsMixer.gain(1, 0); // Noise takes effects mixer slot 1
  effectsMixer.gain(2, 0); // Tones sweep takes effects mixer slot 2
}
