#include "Modes.h"

void startModulation()
{
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
    case TONESWEEP:
      tonesweepOnlyMode();
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
}

void loopModulation()
{
  SoundMode mode = getCurrentMode();

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
    case NONE:
      break;
  }
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
    
    case TONESWEEP:
      setMode(PITCHSHIFT);
      break;    
    
    case PARTY:
      setMode(TONESWEEP);
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
      setMode(TONESWEEP);
      break;
    
    case TONESWEEP:
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

void passthroughMode()
{
  if (getCurrentMode() != NONE)
  {
    flashPixelMulticolor();
    setMode(NONE);
  }
  
  stopPitchShift();
  stopNoise();
  // Tonesweep will stop on its own if not being maintained

  modulationOffLights();
}
