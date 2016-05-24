#include <Adafruit_NeoPixel.h>

#define PIN            2
#define NUMPIXELS      66

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int pixelIndex = 0;
int leds[3] = {22, 0, 0};

void setup() {
  pixels.begin();
}

void loop() {
  for (int i = 0; i < NUMPIXELS; i ++) {
    int activePixel = i + pixelIndex;

    if (activePixel >= NUMPIXELS) {
      activePixel = activePixel - NUMPIXELS;
    }

    pixels.setPixelColor(activePixel, pixels.Color(leds[0],leds[1],leds[2]));
    pixels.show();

    shiftColour(i);
  }

  pixelIndex++;

  if (pixelIndex == NUMPIXELS) {
    pixelIndex = 0;
  }

  delay(0);
}

void shiftColour(int ledIndex) {
  if (ledIndex < 22) {
    leds[0]--;
    leds[1]++;
  } else if (ledIndex >= 22 && ledIndex < 44) {
    leds[1]--;
    leds[2]++;
  } else if (ledIndex >= 44 && ledIndex < 66) {
    leds[2]--;
    leds[0]++;
  }
}
