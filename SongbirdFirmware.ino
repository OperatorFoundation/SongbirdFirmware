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
  playSdAudio(getTestFile());
  #endif

  handleButtons();
  
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
