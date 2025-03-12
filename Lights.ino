#include <Adafruit_NeoPixel.h>  // https://learn.adafruit.com/adafruit-neopixel-uberguide/arduino-library-use

#define LED_1 14 // Blue
#define LED_2 15 // Pink

#define NEOPIXEL_PIN 22
#define NUMPIXELS  1
Adafruit_NeoPixel pixels(NUMPIXELS, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

void setupLights()
{
  // LED
  modulationOffLights();
  pinMode(LED_1, OUTPUT);
  pinMode(LED_2, OUTPUT);

  // Pixel
  pixels.begin();
  pixels.clear();
  pixels.show();
}

void modulationOnLights()
{
  digitalWrite(LED_1, LOW);
  digitalWrite(LED_2, HIGH);
}

void modulationOffLights()
{
  digitalWrite(LED_1, HIGH);
  digitalWrite(LED_2, LOW);
}

/*
* Flash the Neo Pixel through multiple colors
* 
* @param flashDuration (int), time (in ms) each color is displayed. The default is 150 ms.
* @param pauseDuration (int), time (in ms) between colors. The default is 75 ms.
* @param brightness Value between 0-255 to control overall brightness. The default is 40.
*/
void flashPixelMulticolor(int flashDuration = 150, int pauseDuration = 50, int brightness = 40)
{
  uint32_t teal = pixels.Color(0, 128, 128);
  // uint32_t aqua = pixels.Color(0, 255, 255);
  // uint32_t blue = pixels.Color(0, 0, 255);
  uint32_t pink = pixels.Color(255, 105, 180);
  uint32_t purple = pixels.Color(150, 0, 255);
  uint32_t off = pixels.Color(0, 0, 0);

  uint32_t colors[] = {teal, pink, purple};
  int colorCount = 3;

  pixels.setBrightness(brightness);

  // Flash each color in sequence
  for (int i=0; i < colorCount; i++)
  {
    pixels.setPixelColor(0, colors[i]);
    pixels.show();

    delay(flashDuration);

    pixels.setPixelColor(0, off);
    pixels.show();

    delay(pauseDuration);
  }
}
