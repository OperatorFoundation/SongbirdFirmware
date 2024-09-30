#include "Settings.h"

void setupDisplay()
{
    display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
}

void displayText(String text)
{
  // Clear the buffer.
  display.clearDisplay();
  display.display();

  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.println(text);
  display.setCursor(0,0);
  display.display(); // actually display all of the above
  delay(2000);
}

void testDisplay(){
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
