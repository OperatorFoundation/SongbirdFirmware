#include <Adafruit_SSD1306.h>   // https://learn.adafruit.com/monochrome-oled-breakouts/wiring-128x32-i2c-display
#include <Adafruit_GFX.h>       // https://learn.adafruit.com/adafruit-gfx-graphics-library/overview

#include "Modes.h"

// Display
#define SCREEN_WIDTH 128    // OLED display width, in p ixels
#define SCREEN_HEIGHT 32    // OLED display height, in pixels
#define SCREEN_ADDRESS 0x3C // 0x3C for 128x32
#define OLED_RESET     -1   // Reset pin #, if not connected use -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire1, OLED_RESET);  // display is connected to second I2C

boolean dirtyDisplay = true;

void setupDisplay()
{
    display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
}

void refreshDisplay()
{
  dirtyDisplay = true;
}

void displayLines(int lineCount, String text[])
{
  // Clear the buffer.
  display.clearDisplay();
  display.display();

  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);

  for(int index = 0; index < lineCount; index++)
  {
    display.println(text[index]);
  }
  
  display.setCursor(0,0);
  display.display(); // actually display all of the above
}

void displayLine(String text)
{
  display.println(text);
}

void updateDisplay()
{
  if (dirtyDisplay)
  {
    dirtyDisplay = false;
  }
  else
  {
    return;
  }
  
  display.clearDisplay();
  display.display();

  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);

  displayMode(getCurrentMode());

  if (isDevModeEnabled())
  {
    display.println("(DEV)");
    String testFile = getCurrentPlayingFileName();
    display.println(testFile);   
  }
  else
  {
    display.println();
    display.println();
  }

  display.display();
}

void testDisplay()
{
  // print text
  Serial.println(">>> Test display");
  display.display();
  delay(1000);

  // Clear the buffer.
  display.clearDisplay();
  display.display();

  // text display tests
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.print  ("TEST SSD1306 DISPLAY:");
  display.print  ("ABCDEFGHIJKLMNOPQRSTU");
  display.println("0123456789-0123456789");
  display.println(")!@#$%^&*(';:.,[]{}|-");
  display.setCursor(0,0);
  display.display(); // actually display all of the above
  delay(2000);
  display.clearDisplay();
  display.display();
}
