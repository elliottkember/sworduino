#define FASTLED_ALLOW_INTERRUPTS 0
#define FASTLED_ESP8266_NODEMCU_PIN_ORDER
#include <FastLED.h>
#define N_LEDS 48
#define MAX_ALLOWED_MILLIAMPS 5000
#define LED_TYPE WS2812B
#define LED_COLOR_ORDER BGR

namespace Main {
  const int num_routines = 6;
  const char* routines[num_routines] PROGMEM = {
    "Off",
    "Spin",
    "Fast Spin",
    "Slow Spin",
    "Slow Pulse",
    "Blue",
  };
}

void playPattern(int routineName) {
  switch (routineName) {
    case 0:
      black();
      break;
    case 1:
      hexagon();
      break;
    case 2:
      fastHexagon();
      break;
    case 3:
      slowHexagon();
      break;
    case 4:
      slowPulse();
      break;
    case 5:
      blue();
      break;
  }
}

void setup() {
  soulmateSetup();
}

void loop() {
  soulmateLoop();
}
