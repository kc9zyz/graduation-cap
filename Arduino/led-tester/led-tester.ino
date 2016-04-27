#include "FastLED.h"

// How many leds in your strip?
#define NUM_LEDS 3

// For led chips like Neopixels, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806 define both DATA_PIN and CLOCK_PIN
#define DATA_PIN 6
#define CLOCK_PIN 13

// Define the array of leds
CRGB leds[NUM_LEDS];

void setup() { 

  	  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
}

void loop() { 
  static uint8_t val = 0;
  static uint8_t hue = 0;
  if(val > 254)
  {
    val = 0;
    hue += 30;
  }
  FastLED.showColor(CHSV(hue, 255, val++)); 
  delay(1);
}
