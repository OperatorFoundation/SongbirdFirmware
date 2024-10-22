#include "Modes.h"

void startModulation()
{
  SoundMode mode = getCurrentMode();

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
    case NONE:
      passthroughMode();
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

void incrementMode()
{
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
      setMode(NONE);
      break;
    
    case NONE:
      setMode(NOISE);
      break;

     default:
      setMode(NONE);
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
  stopPitchShift();
  stopNoise();
  // Tonesweep will stop on its own if not being maintained
}
