#include "FastLED.h"
#define qsubd(x, b)  ((x>b)?255:0)  // Digital unsigned subtraction macro. if result <0, then => 0. Otherwise, take on fixed value.
#define qsuba(x, b)  ((x>b)?x-b:0)  // Analog Unsigned subtraction macro. if result <0, then => 0
#define LED_DT 7              // Data pin to connect to the strip.
#define COLOR_ORDER GRB       // Are they RGB, GRB or what??
#define LED_TYPE WS2812B      // Don't forget to change LEDS.addLeds
uint8_t max_bright = 255;     // Overall brightness definition. It can be changed on the fly.
unsigned long previousMillis; // Store last time the strip was updated.
int hue = 50;                 // Starting hue.
bool firstTimeRunningThroughPattern = true;

#define NUM_LEDS 1500         // Number of LED's.
struct CRGB leds[NUM_LEDS];   // Initialize our LED array.

#define BEAUTIFUL_SPARKLES 1
#define NIGHT_SPARKLES 2
#define SINGLE_COLOR_SPARKLES 3
#define DAVE 4
#define SHOOTING_GRADIENTS 5
#define DISCO_BARBER_1 6
#define DISCO_TWIRL 7
#define DISCO_BARBER_2 8
#define DISCO_TWIRL_2 9

int maxPatternId = 9;
int rotationInMillseconds = 50000; // 20 seconds for production

// If we're testing one pattern, use holdPattern as true and the patternId as the startingu pattern.
bool holdPattern = false;
int patternId = BEAUTIFUL_SPARKLES;

// Set up LEDs, fade them all to black.
void setup() {
  Serial.begin(57600);
  LEDS.addLeds<LED_TYPE, LED_DT, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(max_bright);
  set_max_power_in_volts_and_milliamps(5, 10000);
  randomSeed(analogRead(0));
  show_at_max_brightness_for_power();
  delay(100);
}

