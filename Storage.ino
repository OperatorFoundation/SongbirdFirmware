#include <SD.h>

#define SDCARD_CS_PIN    BUILTIN_SDCARD
#define SDCARD_MOSI_PIN  7
#define SDCARD_SCK_PIN   14

const int maxTestFiles = 10; // Adjust if more test files are added
String testFileNames[maxTestFiles]; 
int testFileCount = 0;
int currentTestIndex = 0;
String currentPlayingFile = "No test file";

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

    if (dev_mode)
    {
      File root = SD.open("/");
      File testFile = root.openNextFile();

      while (testFile) 
      {
        if (!testFile.isDirectory() && String(testFile.name()).endsWith(".wav"))
        {
          if (testFileCount < maxTestFiles) // Prevents overflow
          {
            testFileNames[testFileCount++] = testFile.name();
          }
        }

        testFile = root.openNextFile(); // Check the next file
      }

      root.close();
    }
  }
}

void playFile()
{
  if (testFileCount == 0)
  {
    Serial.println("No .wav files found.");
    return;
  }

  if (!player.isPlaying()) // Only play the file if we aren't already playing something
  {
    String fileName = testFileNames[currentTestIndex];
    Serial.print("Playing: ");
    Serial.println(fileName);
    currentPlayingFile = fileName;
    player.play(fileName.c_str());

    // Delay to wait for the player to start playing and toggle on the isPlaying() state.
    delay(25);
  }
}

void playNextFile()
{
  if (testFileCount == 0)
  {
    Serial.println("No .wav files found.");
    return;
  }

  if (player.isPlaying())
  {
    player.stop();
  }

  currentTestIndex = (currentTestIndex + 1) % testFileCount; // Select the next index, looping back if we're at the end
  dirtyDisplay = true;

  playFile();
}

String getCurrentPlayingFileName()
{
  return currentPlayingFile;
}

// const char *getTestFile()
// {
//   File root = SD.open("/");
  
//   int index = 0;
//   File firstTest = root.openNextFile();
//   if (!firstTest)
//   {
//     Serial.println("1) Failed to open a test file.");
//     root.close();
//     return 0;        
//   }

//   while (firstTest.name()[0] == '.')
//   {
//     firstTest.close();
//     firstTest = root.openNextFile();

//     if (!firstTest)
//     {
//       Serial.println("2) Failed to open a test file.");
//       root.close();
//       return 0;        
//     }
//   }

//   if(currentTestIndex == 0)
//   {
//     const char *name = firstTest.name();
//     firstTest.close();
//     root.close();
//     return name;
//   }

//   File test;
//   do
//   {
//     test = root.openNextFile();
//     index++;
    
//     if(!test)
//     {
//       Serial.println("3) Failed to open a test file.");

//       const char *name = firstTest.name();
//       root.close();
//       currentTestIndex = 0;
//       firstTest.close();
//       return name;
//     }

//     while (test.name()[0] == '.')
//     {
//       test = root.openNextFile();

//       if(!test)
//       {
        
//         Serial.println("4) Failed to open a test file.");

//         const char *name = firstTest.name();
//         root.close();
//         currentTestIndex = 0;
//         firstTest.close();

//         return name;
//       }
//     }
//   }
//   while(index < currentTestIndex);

//   const char *name = test.name();
//   root.close();
//   firstTest.close();
//   test.close();
//   return name;
// }

// void incrementCurrentTest()
// {
//   player.stop();
//   currentTestIndex++;
//   dirtyDisplay = true;
// }

// void playSdAudio(const char *filename)
// {
//   if (!filename)
//   {
//     return;
//   }
  
//   if (!player.isPlaying())
//   {
//     Serial.print("Playing ");
//     Serial.println(filename);
    
//     player.play(filename);

//     // Delay to wait for the player to start playing and toggle on the isPlaying() state.
//     delay(25);
//   }
// }
