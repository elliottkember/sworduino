#define FASTLED_ALLOW_INTERRUPTS 0
#define FASTLED_ESP8266_NODEMCU_PIN_ORDER
//#define FASTLED_ESP8266_NODEMCU_PIN_ORDER

#include <FastLED.h>

#define N_LEDS 1100
#define DEBUG false
#define MAX_ALLOWED_MILLIAMPS 1000
#define LED_TYPE SK9822
// #define LEDS_WS2812B
#define LEDS_PER_ROW 22
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

  const int num_routines = 8;
  const char* routines[num_routines] PROGMEM = {
    "Flame",
    "Hue-shifting Flame",
    "Lantern",
    "Rain",
    "Worms",
    "Beautiful Sparkles",
    "Dave",
    "Puff"
  };
}

void playPattern(int routine) {
  switch (routine) {
    case 0:
      flame(false, -1, -1);
      break;
    case 1:
      flame(true, 0, 255);
      break;
    case 2:
      lantern();
      break;
    case 3:
      rain();
      break;
    case 4:
      worms();
      break;
    case 5:
      beautiful_sparkles();
      break;
    case 6:
      dave();
      break;
    case 7:
      puff();
      break;
  }
}

void setup() {
  // // wdt_disable();
  soulmateSetup();
}

void loop() {
  soulmateLoop();
}
