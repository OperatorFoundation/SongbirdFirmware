#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <SerialFlash.h>
#include <Bounce2.h>
#include <SD.h>
#include <Audio.h>

#include "Settings.h"

boolean sdFound = false;

// Button Management
Button buttons[numButtons];

enum SoundMode
{
  NOISE, // Left Button
  PITCHSHIFT, // Right Button
  TONESWEEP, // Up Button
  PARTY, // Down Button
  NONE
};

void setup()
{
  setupHardware();
  
  setupPitchShift();

  startModulation();

  Serial.println("setup done");
}

void loop()
{
  // TODO: Record and stop based on button push
  // recordAudio();
  loopModulation();
}

void setupHardware() 
{
  Serial.begin(115200);

  setupButtons();

  setupLights();

  setupStorage();

  setupAudioProcessing();

  setupDisplay();
}
