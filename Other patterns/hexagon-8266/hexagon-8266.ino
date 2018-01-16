#define FASTLED_ALLOW_INTERRUPTS 0
//#define FASTLED_INTERRUPT_RETRY_COUNT 1
#define FASTLED_ESP8266_NODEMCU_PIN_ORDER
#include <FastLED.h>
#include "config.cpp"

namespace Global {
  struct CRGB led_arr[N_CELLS]; //Init LED array
}

// Setup and main loop are in "z_main.ino"

