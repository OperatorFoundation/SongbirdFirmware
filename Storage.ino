#include <SD.h>

#define SDCARD_CS_PIN    BUILTIN_SDCARD
#define SDCARD_MOSI_PIN  7
#define SDCARD_SCK_PIN   14

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
  File root = SD.open("/");
  
  int index = 0;
  File firstTest = root.openNextFile();
  if (!firstTest)
  {
    root.close();
    return 0;        
  }

  while (firstTest.name()[0] == '.')
  {
    firstTest.close();
    firstTest = root.openNextFile();
    if (!firstTest)
    {
      root.close();
      return 0;        
    }
  }

  if(currentTest == 0)
  {
    const char *name = firstTest.name();
    firstTest.close();
    root.close();
    return name;
  }

  File test;
  do
  {
    test = root.openNextFile();
    index++;
    
    if(!test)
    {
      const char *name = firstTest.name();
      root.close();
      currentTest = 0;
      firstTest.close();
      return name;
    }

    while (test.name()[0] == '.')
    {
      test = root.openNextFile();
      if(!test)
      {
        const char *name = firstTest.name();
        root.close();
        currentTest = 0;
        firstTest.close();
        return name;
      }
    }
  }
  while(index < currentTest);

  const char *name = test.name();
  root.close();
  firstTest.close();
  test.close();
  return name;
}

void incrementCurrentTest()
{
  currentTest++;
  dirtyDisplay = true;
}

void playSdAudio(const char *filename)
{
  if (!filename)
  {
    return;
  }
  
  if (!player.isPlaying())
  {
    Serial.print("Playing ");
    Serial.println(filename);
    
    player.play(filename);

    // Delay to wait for the player to start playing and toggle on the isPlaying() state.
    delay(25);
  }
}
