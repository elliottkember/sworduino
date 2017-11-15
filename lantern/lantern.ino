//#define FASTLED_ALLOW_INTERRUPTS 0

#include "FastLED.h"
#include <ArduinoJson.h>

DynamicJsonBuffer jsonBuffer(100);

#define LED_DT 7              // Data pin to connect to the strip.
#define COLOR_ORDER GRB       // Are they RGB, GRB or what??
//#define LED_TYPE APA102      // Don't forget to change LEDS.addLeds
uint8_t MAX_BRIGHT = 255;     // Overall brightness definition. It can be changed on the fly.
uint8_t hue = 50;                 // Starting hue.
bool firstTimeRunningThroughPattern = true;
#define NUM_LEDS 600         // Number of LED's.
struct CRGB leds[NUM_LEDS];   // Initialize our LED array.

void setup() {
  Serial.begin(57600);
  Serial1.begin(9600);
  // LEDS.addLeds<LED_TYPE, LED_DT, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.addLeds<APA102, BGR>(leds, NUM_LEDS).setCorrection(TypicalSMD5050);
  pinMode(7, OUTPUT);
  digitalWrite(7, HIGH);  // enable access to LEDs
//  FastLED.setBrightness(MAX_BRIGHT);
  set_max_power_in_volts_and_milliamps(5.1, 8000);
  randomSeed(analogRead(0));
  FastLED.delay(50);
  FastLED.setDither(0);
  Serial1.setTimeout(10);
}

int speed = 1;

void loop() {
  lanternPattern();
  if(Serial1.available())
  {
    JsonObject& root = jsonBuffer.parseObject(Serial1);
    jsonBuffer.clear();
    float brightness = root["brightness"];
    float hueDecimal = root["hue"];
    float speedDecimal = root["speed"];
    if (hueDecimal > 0) hue = hueDecimal * 255;
    
    if (brightness) {
      brightness = brightness * 255.0;
      if (brightness < 3.0) brightness = 0;
      FastLED.setBrightness(brightness);
    }
    if (speedDecimal) {
      speed = (speedDecimal * 50);
    }
  }
  FastLED.show();
}
