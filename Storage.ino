#define SDCARD_CS_PIN    BUILTIN_SDCARD
#define SDCARD_MOSI_PIN  7
#define SDCARD_SCK_PIN   14

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

void playSdAudio(const char *filename)
{
  if (!player.isPlaying())
  {
    player.play(filename);

    // Delay to wait for the player to start playing and toggle on the isPlaying() state.
    delay(25);
  }
}
