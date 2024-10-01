#include <Adafruit_NeoPixel.h>  // https://learn.adafruit.com/adafruit-neopixel-uberguide/arduino-library-use

// LEDs
#define LED_1 9
#define LED_2 10

#define NEOPIXEL_PIN 22 
#define NUMPIXELS  1
Adafruit_NeoPixel pixels(NUMPIXELS, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

void setupLights()
{
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
}
