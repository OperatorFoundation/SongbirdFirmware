
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include <Bounce2.h>

#include "Settings.h"

boolean sdFound = false;

// Button Management
Bounce2::Button buttons[numButtons];

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
  recordAudio();
  loopModulation();
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

void recordAudio()
{
  // Is the SD card available, and is there data in the queue?
  if (sdFound && audioQueue.available() >= 1)
  {
    // get the data
    int16_t* buffer = audioQueue.readBuffer();

    // Write the data to the SD card
    for (int index = 0; index < AUDIO_BLOCK_SAMPLES; index++)
    {
      audioFile.write((uint8_t *)&buffer[index], sizeof(int16_t));
    }

    audioQueue.freeBuffer();
  }
}

void setupHardware() 
{
  Serial.begin(115200);

  AudioMemory(12);
  audioShield.enable();
  audioShield.volume(0.5);
  audioShield.inputSelect(AUDIO_INPUT_MIC);  // AUDIO_INPUT_LINEIN or AUDIO_INPUT_MIC

  delay(1000);

  // Buttons
  // pull-ups are required, LOW = button pressed, HIGH = not pressed
  for (int index = 0; index < numButtons; index++) 
  {
    // pinMode(buttonPins[index], INPUT_PULLUP); // Set pins as INPUT with internal pull-up resistor
    buttons[index].attach(buttonPins[index], INPUT_PULLUP);  // Attach the switch pin to the Bounce object
    buttons[index].interval(5);            // Set debounce interval (in milliseconds)

    // INDICATE THAT THE LOW STATE CORRESPONDS TO PHYSICALLY PRESSING THE BUTTON
    buttons[index].setPressedState(LOW);
  }

  // LED
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

  // Pixel
  pixels.begin();
  pixels.clear();
  pixels.show();

  // SD Card
  if (!SD.begin(SDCARD_CS_PIN))
  {
    Serial.println("SD Card initialization failed!");
    // Turn Pixel red
    pixels.setPixelColor(0, pixels.Color(10, 0, 0));
    pixels.show();
    sdFound = false;
  }
  else 
  {
    sdFound = true;
  }

  // Audio File
  audioFile = SD.open("songbirdAudio.wav", FILE_WRITE);

  // Mixers
  leftUSBMixer.gain(0, 0.5);
  leftUSBMixer.gain(1, 0.5);
  leftUSBMixer.gain(2, 0.5);
  leftUSBMixer.gain(3, 0.5);
  rightUSBMixer.gain(0, 0.5);
  rightUSBMixer.gain(1, 0.5);
  rightUSBMixer.gain(2, 0.5);
  rightUSBMixer.gain(3, 0.5);

  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
}
