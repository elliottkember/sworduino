#include "FastLED.h"

#define LED_DT 7              // Data pin to connect to the strip.
#define COLOR_ORDER GRB       // Are they RGB, GRB or what??
#define LED_TYPE WS2812B // Don't forget to change LEDS.addLeds
uint8_t MAX_BRIGHT = 255;     // Overall brightness definition. It can be changed on the fly.
uint8_t hue = 50;                 // Starting hue.
bool firstTimeRunningThroughPattern = true;
#define NUM_LEDS 600         // Number of LED's.
struct CRGB leds[NUM_LEDS];   // Initialize our LED array.

void setup() {
  Serial.begin(57600);
  LEDS.addLeds<LED_TYPE, LED_DT, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(MAX_BRIGHT);
  set_max_power_in_volts_and_milliamps(5, 1000);
  pinMode(14, INPUT_PULLUP);
  digitalWrite(14, HIGH);
  randomSeed(analogRead(0));
  delay(50);
}

int on = true;
int numberOfLoopsPressed = 0;
int switchMode = 1;
int numberOfBrightnesses = 6;
int brightnesses [6] = { 0, 32, 64, 128, 200, 255 };
int brightnessIndex = 128;
uint8_t bright = 1;
int16_t newBright = brightnesses[brightnessIndex];

int getLevel() {
  switchMode = digitalRead(14);

  if (switchMode == 0) { 
    numberOfLoopsPressed++;
  } else {
    if (numberOfLoopsPressed < 30 && numberOfLoopsPressed > 0) {
      brightnessIndex = (brightnessIndex + 1) % 6;
      newBright = brightnesses[brightnessIndex];
    }
    if (bright < newBright) {
      bright += min(255 - bright, 5);
    } else if (newBright == 0 && bright > 0) {
      bright -= min(bright, 5);
    }
    numberOfLoopsPressed = 0;
  }

  if (bright > 255) bright = 255;
  if (bright < 0) bright = 0;

  return bright;
}

void loop () {
  Serial.println("a");
  lanternPattern(brightnessIndex);
//  fill_solid(leds, NUM_LEDS, CHSV(hue, 255, 255));
//  hue++;
//  Serial.println(hue);
  FastLED.setBrightness(255);
  FastLED.show();
}


