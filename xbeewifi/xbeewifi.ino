#include <ArduinoJson.h>
#include <FastLED.h>

struct CRGB leds[10];
DynamicJsonBuffer jsonBuffer(200);

void setup() {
  randomSeed(analogRead(0));
  Serial.begin(57600);
  Serial1.begin(9600);
  FastLED.addLeds<APA102, 11, 13, GRB>(leds, 10).setCorrection(TypicalSMD5050);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 500);
  delay(1000);
}

void loop() {
  if(Serial1.available()) {
    JsonObject& root = jsonBuffer.parseObject(Serial1);
    float brightness = root["brightness"];
    int hue = (float)root["hue"] * 255;
    int patternId = root["patternId"];
    float speed = root["speed"];
    
    // if (brightness) FastLED.setBrightness(brightness * 255);
  }
  FastLED.show();
}
