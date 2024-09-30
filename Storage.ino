void setupStorage()
{
   // SD Card
  if (!SD.begin(SDCARD_CS_PIN))
  {
    Serial.println("SD Card initialization failed!");
    // Turn Pixel red
    // TODO: Blink for errors
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
}
