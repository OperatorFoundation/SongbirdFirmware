#include "Modes.h"

boolean dev_mode = false;

boolean isDevModeEnabled()
{
  return dev_mode;
}

void toggleDevMode()
{
  if (dev_mode)
  {
    dev_mode = false;
    Serial.println("Production mode");
  }
  else
  {
    dev_mode = true;
    Serial.println("Dev mode");
  }

  setProductionDevMixer();
  refreshDisplay();
}

void handleSerialInput()
{
  if (!Serial.available()) { return; }

  int input = Serial.read();

  if (input == -1) { return; }

  char inputChar = (char)input;

  Serial.print("Received Serial input: ");
  Serial.println(inputChar);

  switch (inputChar)
  {
    case 'n': // noise mode requested
      Serial.println("Starting Noise Mode");
      setMode(NOISE);
      startModulation();
      break;
    case 'p':
      Serial.println("Starting Pitch Shift Mode");
      setMode(PITCHSHIFT);
      startModulation();
      return;
    case 'w':
      Serial.println("Starting Wave Form Mode");
      setMode(WAVEFORM);
      startModulation();
      return;
    case 'a':
      Serial.println("Starting Party Mode");
      setMode(PARTY);
      startModulation();
      return;
    default:
      return;
  }
}
