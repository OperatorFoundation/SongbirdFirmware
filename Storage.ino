#define SDCARD_CS_PIN    BUILTIN_SDCARD
#define SDCARD_MOSI_PIN  7
#define SDCARD_SCK_PIN   14

const int numTests = 10;
const char *tests[numTests][11] = {
  "test1_1.wav",  
  "test1_2.wav",  
  "test1_3.wav",  
  "test2_1.wav",  
  "test2_2.wav",  
  "test2_3.wav",  
  "test3_1.wav",  
  "test3_2.wav",  
  "test3_3.wav",  
  "test4_1.wav",
  "test4_2.wav",
  "test4_3.wav",
};

int currentTest = 0;

void setupStorage()
{
  SPI.setMOSI(SDCARD_MOSI_PIN);
  SPI.setSCK(SDCARD_SCK_PIN);
  if (!(SD.begin(SDCARD_CS_PIN)))
  {
    Serial.println("SD Card initialization failed!");
    pixels.setPixelColor(0, pixels.Color(10, 0, 0));
    pixels.show();
    sdFound = false;
  }
  else
  {
    sdFound = true;
  }
}

const char *getTestFile()
{
  return *tests[currentTest];
}

void incrementCurrentTest()
{
  currentTest++;

  if (currentTest >= numTests)
  {
    currentTest = 0;
  }

  Serial.print("Playing test file ");
  Serial.println(getTestFile());
  dirtyDisplay = true;
}

void playSdAudio(const char *filename)
{
  if (!player.isPlaying())
  {
    player.play(filename);

    // Delay to wait for the player to start playing and toggle on the isPlaying() state.
    delay(25);
  }
}
