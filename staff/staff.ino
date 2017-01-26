#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

#include "FastLED.h"
#include "constants.cpp"

const char* ssid = "UENO.";
const char* password = "haraldur";

namespace Global {
  struct CRGB leds[NUM_LEDS];
  bool firstTimeRunningThroughPattern = false;
  int hue = 50;
  uint8_t max_bright = 255;

  void printPixels(CRGB (*calculatePixel)(int)) {
    for (int k = 0; k < NUM_LEDS - 1; k++) {
      leds[k] = calculatePixel(k);
    }
  }
}

#define SWITCH_PIN 12

void setup() {
//  ESP.wdtDisable();

  pinMode(SWITCH_PIN, INPUT_PULLUP);
  randomSeed(analogRead(0));
  Serial.begin(57600);
  LEDS.addLeds<LED_TYPE, LED_DT, COLOR_ORDER>(Global::leds, NUM_LEDS);
  set_max_power_in_volts_and_milliamps(VOLTS, MAX_CURRENT_IN_MA);
  delay(100);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }

  // Port defaults to 8266
//   ArduinoOTA.setPort(8266);1

  // Hostname defaults to esp8266-[ChipID]
//   ArduinoOTA.setHostname("myesp8266");

  // No authentication by default
   ArduinoOTA.setPassword((const char *)"123");

  ArduinoOTA.onStart([]() {
    Serial.println("Start");
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}
