#define FASTLED_ALLOW_INTERRUPTS 0
#define FASTLED_ESP8266_NODEMCU_PIN_ORDER

#include <FastLED.h>

#define N_LEDS 48
#define DEBUG false
#define MAX_ALLOWED_MILLIAMPS 10000
#define LED_TYPE WS2812B
#define LEDS_WS2812B true
// #define LED_COLOR_ORDER BGR
#define LEDS_PER_ROW 9
#define N_CELLS (N_LEDS + N_LEDS % LEDS_PER_ROW)

// TODO: These should be standardized and out of htere
#define FPS 60
#define DELAY (1000/FPS) // in milliseconds

namespace Util {
  int rand_range(int min, int max) {
    return random(min, (max + 1));
  }
}

namespace Soulmate {
  struct CRGBArray<N_CELLS> led_arr;

  const int num_routines = 4;
  const char* routines[num_routines] PROGMEM = {
    "Spin",
    "Fast Spin",
    "Slow Spin",
    "Slow Pulse",
  };
}

void playPattern(int routine) {
  Serial.println(routine);
  switch (routine) {
    case 0:
      hexagon();
      break;
    case 1:
      fastHexagon();
      break;
    case 2:
      slowHexagon();
      break;
    case 3:
      slowPulse();
      break;
  }
}

void setup() {
  soulmateSetup();
}

void loop() {
  soulmateLoop();
}
