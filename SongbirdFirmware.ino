#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <SerialFlash.h>
#include <Bounce2.h>
#include <SD.h>
#include <Audio.h>

#include "Settings.h"
#include "config.h"

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
  Serial.begin(115200);
  Serial.println("Setting up...");
  
  setupHardware();
  
  setupPitchShift();

  startModulation();

  Serial.println("setup done");
}

void loop()
{
  #ifdef DEV_MODE
  playSdAudio("test1.wav");
  #endif
  
  loopModulation();
}

void setupHardware() 
{
  setupButtons();

  setupLights();

  setupStorage();

  setupAudioProcessing();

  setupDisplay();
}
