/*
 * Project arneblinking
 * Description: simple neopixel controller
 * Author: derMicha
 * Date: 25.11.2017
 */

/*
 * https://docs.particle.io/datasheets/photon-(wifi)/photon-datasheet/#technical-specification
 * https://learn.adafruit.com/adafruit-neopixel-uberguide/basic-connections
 * https://github.com/technobly/Particle-NeoPixel
*/

#include "Particle.h"
#include "neopixel.h"

//SYSTEM_MODE(MANUAL);
SYSTEM_MODE(AUTOMATIC);
SYSTEM_THREAD(ENABLED);

// IMPORTANT: Set pixel COUNT, PIN and TYPE
#define PIXEL_PIN D4
#define PIXEL_COUNT 50
#define PIXEL_TYPE WS2812B

/* ======================= prototypes =============================== */
void colorAll(uint32_t c, uint8_t wait);
void colorWipe(uint32_t c, uint8_t wait);
void rainbow(uint8_t wait);
void rainbowCycle(uint8_t wait);
uint32_t Wheel(byte WheelPos);

Adafruit_NeoPixel strip(PIXEL_COUNT, PIXEL_PIN, PIXEL_TYPE);

// controlling led state
// default state
bool npState = false;
int swNeopixel(String extra);
void switchAllNeopixelOff();

// setup() runs once, when the device is first turned on.
void setup() {
  Particle.connect();

  Particle.variable("npState", npState);
  Particle.function("swNeopixel", swNeopixel);

  strip.begin();
  switchAllNeopixelOff();
  strip.setBrightness(128);
  strip.show();
}

// loop() runs over and over again, as quickly as it can execute.
void loop() {
    if(npState == true) {
      //strip.setPixelColor(0, strip.Color(255, 0, 255));
      //strip.show();

      //colorWipe(strip.Color(255, 0, 0), 50); // Red

      //colorWipe(strip.Color(0, 255, 0), 50); // Green

      //colorWipe(strip.Color(0, 0, 255), 50); // Blue

      //rainbow(20);

      rainbowCycle(20);

      //colorAll(strip.Color(0, 255, 255), 50); // Cyan
    }
    else {
      switchAllNeopixelOff();
    }

    delay(200);
}

// switches led state
int swNeopixel(String extra) {
  npState = !npState;
  Particle.publish("switched Neopixel state", npState ? "on" : "off", 30, PRIVATE);
  return npState;
}

// switches all leds o, ff
void switchAllNeopixelOff() {
  for(uint16_t i=0; i <strip.numPixels(); i++) {
    strip.setPixelColor(i, 0, 0, 0);
  }
  strip.show();
}

// neopixel controlling funtions
// Set all pixels in the strip to a solid color, then wait (ms)
void colorAll(uint32_t c, uint8_t wait) {
  uint16_t i;

  for(i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
  }
  strip.show();
  delay(wait);
}

// Fill the dots one after the other with a color, wait (ms) after each one
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    if(!npState) {
      break;
    }
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout, then wait (ms)
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) { // 1 cycle of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
      if(!npState) {
        break;
      }
    }
    strip.show();
    delay(wait);
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
   return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}
