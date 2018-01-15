#define FASTLED_ALLOW_INTERRUPTS 0
//#define FASTLED_INTERRUPT_RETRY_COUNT 1
#define FASTLED_ESP8266_NODEMCU_PIN_ORDER
#include <FastLED.h>
#include "config.cpp"

#define N_LEDS 48 // Number of LEDs on our light strip // Max seems to be 1520 on ESP32
#define DEBUG false
#define LEDS_PER_ROW 9
#define MAX_ALLOWED_MILLIAMPS 5000
#define LED_TYPE WS2812B
#define LED_COLOR_ORDER BGR

namespace Global {
  struct CRGB led_arr[N_CELLS]; //Init LED array
}

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
