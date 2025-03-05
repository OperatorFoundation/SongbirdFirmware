#include <Adafruit_NeoPixel.h>  // https://learn.adafruit.com/adafruit-neopixel-uberguide/arduino-library-use

// LED1 is Blue and LED2 is pink
#define LED_1 14
#define LED_2 15

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

  // Pixel
  pixels.begin();
  pixels.clear();
  pixels.show();
}
