#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include <Bounce2.h>

#include "Settings.h"

// Button Management
Bounce debouncers[numButtons];
bool buttonStates[numButtons] = {false, false, false, false}; // Initialize states to LOW
int lastButtonPressed = -1; // Track the last button pressed

enum SoundMode
{
  NOISE, // Left Button
  PITCHSHIFT, // Right Button
  TONESWEEP, // Up Button
  PARTY, // Down Button
  NONE
};

// Function to get the current mode based on the last button pressed
SoundMode getCurrentMode() 
{
  switch (lastButtonPressed) 
  {
    case 0: // Button 0 - Left Button: NOISE
      displayText("Noise Mode");
      return NOISE;
    case 1: // Button 1 - Right Button: PITCHSHIFT
      displayText("Pitch Shift");
      return PITCHSHIFT;
    case 2: // Button 2 - Up Button: TONESWEEP
      displayText("Tone Sweep");
      return TONESWEEP;
    case 3: // Button 3 - Down Button: PARTY
      displayText("PARTY MODE!!");
      return PARTY;
    default: // No mode if no button is pressed
      displayText("No Mode Selected");
      return NONE;
  }
}

void setup() 
{
  setupHardware();
  
  setupPitchShift();

  startModulation();

  Serial.println("setup done");
}

void setupHardware() 
{
  Serial.begin(115200);

  AudioMemory(12);
  audioShield.enable();
  audioShield.volume(0.5);
  audioShield.inputSelect(AUDIO_INPUT_MIC);  // AUDIO_INPUT_LINEIN or AUDIO_INPUT_MIC

  delay(1000);

  // pull-ups are required, LOW = button pressed, HIGH = not pressed
  for (int index = 0; index < numButtons; index++) {
    pinMode(buttonPins[index], INPUT_PULLUP); // Set pins as INPUT with internal pull-up resistor
    debouncers[index].attach(buttonPins[index]);  // Attach the switch pin to the Bounce object
    debouncers[index].interval(50);            // Set debounce interval (in milliseconds)
  }

  digitalWrite(LED_1, LOW);
  digitalWrite(LED_2, LOW);
  pinMode(LED_1, OUTPUT);
  pinMode(LED_2, OUTPUT);

  digitalWrite(HPAMP_VOL_CLK,  LOW);
  digitalWrite(HPAMP_VOL_UD,   LOW);
  digitalWrite(HPAMP_SHUTDOWN, HIGH); // disable headphone amp by setting HIGH
  pinMode(HPAMP_VOL_CLK,  OUTPUT);
  pinMode(HPAMP_VOL_UD,   OUTPUT);
  pinMode(HPAMP_SHUTDOWN, OUTPUT);

  pixels.begin();
  pixels.clear();
  pixels.show();

  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
}

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
      // TODO: Handle NONE case
      Serial.println("Current sound mode is NONE. This is not yet implemented. Doing nothing.");
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
      // TODO: Handle NONE case
      Serial.println("Current sound mode is NONE");
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

void loop()
{
  loopModulation();
}

void updateButtons()
{
    // Update each Bounce object
  for (int index = 0; index < numButtons; index++) 
  {
    debouncers[index].update();  // Update the state of the Bounce object

    // Check if the switch state has changed
    if (debouncers[index].fell()) 
    {   // Triggered when switch is pressed (transition from HIGH to LOW)
      buttonStates[index] = true;    // Update the state
      lastButtonPressed = index;     // Update the last button pressed
      Serial.print("Button ");
      Serial.print(index);
      Serial.println(" pressed.");
    }

    if (debouncers[index].rose()) 
    {   // Triggered when switch is released (transition from LOW to HIGH)
      buttonStates[index] = false;   // Update the state
    }
  }
}
