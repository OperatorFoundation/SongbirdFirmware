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

// Function to get the current mode based on the last button pressed
SoundMode getCurrentMode()
{
  // Update each Bounce object
  for (int index = 0; index < numButtons; index++) 
  {
    buttons[index].update();  // Update the state of the Bounce object

    if (buttons[index].released()) { // Button was just released
      // Serial.print("Button ");
      // Serial.print(index);
      // Serial.println(" was just released.");

      switch (index) 
      {
        case 0: // Button 0 - Left Button: NOISE
          Serial.println("Noise Mode Selected");
          displayText("Noise Mode");
          return NOISE;
        case 1: // Button 1 - Right Button: PITCHSHIFT
          Serial.println("Pitch Shift Selected");
          displayText("Pitch Shift");
          return PITCHSHIFT;
        case 2: // Button 2 - Up Button: TONESWEEP
          Serial.println("Tone Sweep Selected");
          displayText("Tone Sweep");
          return TONESWEEP;
        case 3: // Button 3 - Down Button: PARTY
          Serial.println("PARTY MODE!!");
          displayText("PARTY MODE Selected!!");
          return PARTY;
        default: // No mode if no button is pressed
          Serial.println("No Mode Selected");
          displayText("No Mode Selected");
          return NONE;
      }
    }
  }

  return NONE;
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
