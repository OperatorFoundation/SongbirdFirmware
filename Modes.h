#ifndef _MODES_H_
#define _MODES_H_

enum SoundMode
{
  NOISE,
  PITCHSHIFT,
  WAVEFORM,
  PARTY,
  NONE
};

SoundMode currentMode = NONE;

// In order to have a function that returns an enum type, the enum type definition and the function definition must be in a .h file. This is a side effect of Arduino code reordering.
SoundMode getCurrentMode()
{
  return currentMode;
}

void setMode(SoundMode newMode)
{
  currentMode = newMode;
  switch (currentMode)
  {
    case NOISE:
      Serial.println("Noise Mode Selected");
      break;
           
    case PITCHSHIFT:
      Serial.println("Pitch Shift Selected");
      break;
    
    case WAVEFORM:
      Serial.println("Waveform Selected");
      break;    
    
    case PARTY:
      Serial.println("PARTY MODE!!");
      break;
    
    case NONE:
      Serial.println("No Mode Selected");
      break;

     default:
      Serial.println("No Mode Selected");
      break;   
  }

  refreshDisplay();
}

void displayMode(SoundMode mode)
{
  switch (mode)
  {
    case NOISE:
      displayLine("Noise Mode");
      break;
           
    case PITCHSHIFT:
      displayLine("Pitch Shift");
      break;
    
    case WAVEFORM:
      displayLine("Waveform");      
      break;    
    
    case PARTY:
      displayLine("PARTY MODE Selected!!");
      break;
    
    case NONE:
      displayLine("No Mode Selected");      
      break;

     default:
      displayLine("No Mode Selected");      
      break;   
  }
}

#endif
