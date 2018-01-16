#define FASTLED_ALLOW_INTERRUPTS 0
#define FASTLED_ESP8266_NODEMCU_PIN_ORDER

#include <FastLED.h>

#define N_LEDS 48
#define DEBUG false
#define MAX_ALLOWED_MILLIAMPS 10000
#define LED_TYPE WS2812B
#define LEDS_WS2812B
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

  const int num_routines = 18;
  const char* routines[num_routines] PROGMEM = {
    "White",
    "Warm white",
    "Flame",
    "Dim flame",
    "Hue-shifting flame",
    "Laser spool",
    "Rainbow laser spool",
    "Conway",
    "Flame crystallization",
    "Puff",
    "Rainbow",
    "Pulse",
    "Slow pulse",
    "Slowest pulse",
    "Beautiful sparkles",
    "Shooting gradients",
    "Dave",
    "Rain"
  };
}

void playPattern(int routine) {
  switch (routine) {
    case 0: white();                    break; // "White",
    case 1: warm_white();               break; // "Warm white",
    case 2: flame(false, -1, -1);       break; // "Flame",
    case 3: flame(false, 0, 64);        break; // "Dim flame",
    case 4: flame(true, 0, 255);        break; // "Hue-shifting flame",
    case 5: spool();                    break; // "Laser spool",
    case 6: rainbow_spool();            break; // "Rainbow laser spool",
    case 7: conway();                   break; // "Conway",
    case 8: flame_crystallization();    break; // "Flame crystallization",
    case 9: puff();                    break; // "Puff",
    case 10: rainbow();                 break; // "Rainbow",
    case 11: pulse();                   break; // "Pulse",
    case 12: slowPulse();               break; // "Slow pulse",
    case 13: slowestPulse();            break; // "Slowest pulse",
    case 14: beautiful_sparkles();      break; // "Beautiful sparkles",
    case 15: shooting_gradients();      break; // "Shooting gradients",
    case 16: dave();                    break; // "Dave",
    case 17: rain();                    break; // "Rain"
  }
}

void setup() {
  soulmateSetup();
}

void loop() {
  soulmateLoop();
}
