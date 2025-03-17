#include "Modes.h"

void startModulation()
{
  AudioInterrupts();

  SoundMode mode = getCurrentMode();

  switch (mode) 
  {
    case NOISE:
      noiseOnlyMode();
      modulationOnLights();
      break;
    case PITCHSHIFT:
      shiftOnlyMode();
      modulationOnLights();
      break;
    case WAVEFORM:
      waveformOnlyMode();
      modulationOnLights();
      break;
    case PARTY:
      partyMode();
      modulationOnLights();
      break;
    case NONE:
      passthroughMode();
      modulationOffLights();
      break;
  }

  AudioNoInterrupts();
}

void loopModulation()
{
  AudioInterrupts();

  SoundMode mode = getCurrentMode();

  switch (mode)
  {
    case NOISE:
      maintainNoise();
      break;
    case PITCHSHIFT:
      maintainShift();
      break;
    case WAVEFORM:
      maintainWaveform();
      break;
    case PARTY:
      maintainParty();
      break;
    case NONE:
      break;
  }
  
  AudioNoInterrupts();
}

void previousMode()
{
  flashPixelMulticolor();

  switch (currentMode)
  {
    case NOISE:
      setMode(PARTY);
      break;
           
    case PITCHSHIFT:
      setMode(NOISE);
      break;
    
    case WAVEFORM:
      setMode(PITCHSHIFT);
      break;    
    
    case PARTY:
      setMode(WAVEFORM);
      break;

    default:
      setMode(NOISE);
      break;      
  }

  startModulation();
}

void incrementMode()
{
  flashPixelMulticolor();

  switch (currentMode)
  {
    case NOISE:
      setMode(PITCHSHIFT);
      break;
           
    case PITCHSHIFT:
      setMode(WAVEFORM);
      break;
    
    case WAVEFORM:
      setMode(PARTY);
      break;    
    
    case PARTY:
      setMode(NOISE);
      break;

    default:
      setMode(NOISE);
      break;      
  }

  startModulation();
}

void shiftOnlyMode()
{
  stopEveryMode();
  startPitchShift();
}

void noiseOnlyMode()
{
  stopEveryMode();
  startNoise();
}

void waveformOnlyMode()
{
  stopEveryMode();
  startWaveform();
}

void partyMode()
{
  startNoise();
  startPitchShift();
  startWaveform();
}

void maintainParty()
{
  maintainWaveform();
  maintainNoise();
  maintainShift();
}

void passthroughMode()
{
  if (getCurrentMode() != NONE)
  {
    flashPixelMulticolor();
    setMode(NONE);
  }
  
  stopEveryMode();
  modulationOffLights();
}

void stopEveryMode()
{
  stopPitchShift();
  stopNoise();
  stopWaveform();
}
