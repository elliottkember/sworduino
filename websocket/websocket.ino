#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESP8266mDNS.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WebSocketsServer.h>
#include <Hash.h>
#include <FastLED.h>

// Constants
#define FASTLED_ALLOW_INTERRUPTS 0
#define LED_PIN     7
#define COLOR_ORDER GRB
#define CHIPSET     WS2811
#define NUM_LEDS    150
#define FPS         30

// WIFI
const char* ssid = "";
const char* password = "";

// brightness
uint8_t max_bright = 80;

// Timers
unsigned long render = 0;

// Setup libraries
ESP8266WiFiMulti WiFiMulti;
CRGB leds[NUM_LEDS];
WebSocketsServer webSocket = WebSocketsServer(81);

/**
 * Websocket event
 *
 * @param {num} Client number
 * @param {type} Command type (disconnect, connect, read)
 * @param {payload} Received payload
 * @param {len} Payload size
 */
void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t len) {
  switch (type) {

    case WStype_DISCONNECTED: {
      Serial.printf("[%u] Disconnected!\n", num);
    } break;

    case WStype_CONNECTED: {
      IPAddress ip = webSocket.remoteIP(num);
      Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
    } break;

    case WStype_TEXT: {
      String command = (char*) payload;
      if (command == "status") {
        /**
         * Send status to client
         */

        String command = (char*) payload;
        StaticJsonBuffer<1024> jsonBuffer;

        JsonObject& root = jsonBuffer.createObject();
        root['brightness'] = max_bright;
        root['patternId'] = patternId;

        String json;
        root.printTo(json);

        webSocket.sendTXT(num, json);
        return;
      }

      StaticJsonBuffer<1024> jsonBuffer;
      JsonObject& root = jsonBuffer.parseObject(command);

      int pattern = root["pattern"];
      if (pattern) patternId = pattern;

      int brightness = root["brightness"];
      if (brightness) max_bright = brightness;
    }

    case WStype_BIN: {
      hexdump(payload, len);
      uint16_t i = ((uint16_t)payload[0] << 8) | payload[1];
      patternId = i;
    } break;
  }
}

void setupWiFi() {
  WiFiMulti.addAP(ssid, password);

  Serial.println();
  Serial.println();
  Serial.print("Wait for WiFi");

  while (WiFiMulti.run() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  delay(500);
}

void setup() {
  // Serial
  Serial.begin(115200);
  delay(10);

  // Setup WiFi
  setupWiFi();

  // Websocket
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);

  // FastLED
  LEDS.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(255);
}

void loop() {
  FastLED.setBrightness(max_bright);
  webSocket.loop();
  if (millis() > render) {
    FastLED.show();
    render = millis() + (1000 / FPS);
  }
}
