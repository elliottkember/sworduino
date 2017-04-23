
// Uncomment when using an ESP8266, 
// and call setupWifi(); in setup!

//#include <Arduino.h>
//#include <ArduinoJson.h>
//#include <ESP8266mDNS.h>
//#include <ESP8266WiFi.h>
//#include <ESP8266WiFiMulti.h>
//#include <WebSocketsServer.h>
//#include <Hash.h>
//#include <FastLED.h>
//
//const char* ssid = "UENO";
//const char* password = "haraldur";
//ESP8266WiFiMulti WiFiMulti;
//WebSocketsServer webSocket = WebSocketsServer(81);
// 
//namespace Socketry {
//  void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t len) {
//    switch (type) {
//  
//      case WStype_DISCONNECTED: {
//        Serial.printf("[%u] Disconnected!\n", num);
//        webSocket.begin();
//      } break;
//  
//      case WStype_CONNECTED: {
//        IPAddress ip = webSocket.remoteIP(num);
//        Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
//      } break;
//  
//      case WStype_TEXT: {
//        String command = (char*) payload;
//        Serial.println(command);
//  
//        StaticJsonBuffer<1024> jsonBuffer;
//        JsonObject& root = jsonBuffer.parseObject(command);
//  
//        if (root["pattern"]) {
//          Global::firstTimeRunningThroughPattern = true;
//          Global::patternId = root["pattern"];
//        }
//  
//        if (root["brightness"]) {
//          Global::max_bright = root["brightness"];
//        }
//  
//        if (root["speed"]) {
//          Global::frameDelay = root["speed"];
//        }
//  
//        String message = "{\"speed\": " + (String)Global::frameDelay + ", \"brightness\": " + (String)Global::max_bright + ", \"pattern\": " + (String)Global::patternId + "}";
//        webSocket.sendTXT(num, message);
//      }
//    }
//  }
//  
//  void setupWiFi() {
//    WiFiMulti.addAP(ssid, password);
//  
//    Serial.println();
//    Serial.println();
//    Serial.print("Wait for WiFi");
//  
//    while (WiFiMulti.run() != WL_CONNECTED) {
//      Serial.print(".");
//      delay(500);
//    }
//  
//    Serial.println("");
//    Serial.println("WiFi connected");
//    Serial.print("IP address: ");
//    Serial.println(WiFi.localIP());
//  
//    delay(500);
//  
//    // Websocket
//    webSocket.begin();
//    webSocket.onEvent(Socketry::webSocketEvent);
//  }
//}
//
//void setupWifi() {
//  Socketry::setupWiFi();
//}

