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
      currentMode = PITCHSHIFT;
      Serial.println("Pitch Shift Selected");
      displayText("Pitch Shift");
      break;
           
    case PITCHSHIFT:
      currentMode = TONESWEEP;
      Serial.println("Tone Sweep Selected");
      displayText("Tone Sweep");      
      break;
    
    case TONESWEEP:
      currentMode = PARTY;
      Serial.println("PARTY MODE!!");
      displayText("PARTY MODE Selected!!");
      break;    
    
    case PARTY:
      currentMode = NONE;
      Serial.println("No Mode Selected");
      displayText("No Mode Selected");      
      break;
    
    case NONE:
      currentMode = NOISE;
      Serial.println("Noise Mode Selected");
      displayText("Noise Mode");
      break;

     default:
      currentMode = NONE;
      Serial.println("No Mode Selected");
      displayText("No Mode Selected");      
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

void passthroughMode()
{
  stopPitchShift();
  stopNoise();
  // Tonesweep will stop on its own if not being maintained
}
