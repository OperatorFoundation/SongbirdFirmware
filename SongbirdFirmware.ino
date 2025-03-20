#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <SerialFlash.h>
#include <Bounce2.h>
#include <SD.h>
#include <Audio.h>

// OpenAudio includes
#include "AudioStream_F32.h"
#include <OpenAudio_ArduinoLibrary.h>

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

void setupHardware() 
{
  setupButtons();

  setupLights();

  setupStorage();

  setupAudioProcessing();

  setupDisplay();
}

void loop()
{
  if (isDevModeEnabled())
  {
    playFile();

    // const char *testFile = getTestFile();
    // if (testFile)
    // {
      
    //   playSdAudio(testFile);
    // }
  }

  handleButtons();
  
  loopModulation();

  updateDisplay();
}
