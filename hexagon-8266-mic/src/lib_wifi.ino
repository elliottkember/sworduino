// #ifndef LIB_WIFI
// #define LIB_WIFI
//
// #if defined(ESP32) || defined(ESP8266)
//
// #if defined(ESP32)
//   #include <WiFi.h>
//   #include <WebServer.h>
//   #include <ESPmDNS.h>
// #elif defined(ESP8266)
//   #include <ESP8266WiFi.h>
//   #include <ESP8266WebServer.h>
//   #include <ESP8266mDNS.h>
// #endif
//
// #include <DNSServer.h>
// #include <WiFiClientSecure.h>
// #include <WiFiManager.h>
//
// #if defined(ESP32)
//   WebServer server(80);
// #elif defined(ESP8266)
//   ESP8266WebServer server(80);
// #endif
//
// #include <WiFiUdp.h>
// #include <ArduinoJson.h>
//
// namespace Wifi {
//   const int LED_ON = LOW;
//   const int LED_OFF = HIGH;
//   const int led = BUILTIN_LED;
//   const unsigned long heartBeatEvery = 7200000; // Checks in with the server every N milliseconds
//   unsigned long lastHeartBeat; // Millisecond timestamp at time of last heartbeat
//   //WiFiClient client;
//   WiFiClientSecure client;
//
//   // UDP
//   WiFiUDP Udp;
//   unsigned int localUDPPort = 12345;
//   char incomingPacket[255];
//
//   bool changed = false;
//
//   void handleNotFound(){
//     digitalWrite(led, LED_ON);
//     String message = "File Not Found\n\n";
//     message += "URI: ";
//     message += server.uri();
//     message += "\nMethod: ";
//     message += (server.method() == HTTP_GET)?"GET":"POST";
//     message += "\nArguments: ";
//     message += server.args();
//     message += "\n";
//     for (uint8_t i=0; i<server.args(); i++){
//       message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
//     }
//     server.send(404, "text/plain", message);
//     digitalWrite(led, LED_OFF);
//   }
//
//   void handleRoot() {
//     digitalWrite(led, LED_ON);
//     server.send(200, "text/plain", String("hello from esp with MAC " + WiFi.macAddress() + "! Running routine #" + CurrentRoutine()));
//     digitalWrite(led, LED_OFF);
//     black();
//   }
//
//   void handleChooseRoutine() {
//     if (server.arg("routine") == "") {
//       // Invalid
//       //server.send(404, "text/plain", "Not found");
//       handleNotFound();
//       return;
//     }
//
//     String routine = server.arg("routine");
//     const char* chosen = ChooseRoutine(routine.toInt());
//     server.send(200, "text/plain", String("Selected ") + chosen);
//   }
//
//   void handleBrightness() {
//     String brightness_submitted = server.arg("brightness");
//     if(brightness_submitted.length() > 0) {
//       // Update it
//       ChooseBrightness((uint8_t)brightness_submitted.toInt());
//     }
//
//     String brightness = String(CurrentBrightness());
//     server.send(200, "text/plain", brightness);
//   }
//
//   void handleListRoutines() {
//     String message = "{ \"CurrentRoutine\": " + CurrentRoutine() + ", \"Routines\": [ ";
//
//     const char** routines = Routines();
//     int i = 0;
//     for (;i < NumRoutines()-1; i++) {
//       //Serial.println(routines[i]);
//       message += "\"" + String(routines[i]) + "\", ";
//     }
//
//     // (Note that i was already incremented by the loop, no need to increment again)
//     message += "\"" + String(routines[i]) + "\" ] }";
//
//     server.send(200, "application/json", message);
//   }
//
//   void handleReset() {
//     WiFi.disconnect(true);
//     delay(1000);
//     ESP.restart();
//   }
//
//   void checkin() {
//     String data = String("firmware=1.0")
//       + String("&lan_ip=") + String(WiFi.localIP()[0]) + "." + String(WiFi.localIP()[1]) + "." + String(WiFi.localIP()[2]) + "." + String(WiFi.localIP()[3])
//       + String("&mac=") + String(WiFi.macAddress());
//
//     //IPAddress host = IPAddress(192,168,1,47);
//     //uint16_t port = 9019;
//     const char* host = "soulmate.carbocation.com";
//     uint16_t port = 443;
//
//     if (!client.connect(host, port)) {
//       Serial.println("checkin failed");
//       return;
//     }
//
//     client.println("POST /soulmate/v1/checkin HTTP/1.1");
//     //client.println("Host: test.com");
//     client.println(String("Host: ") + String(host));
//     client.println("User-Agent: Soulmate/1.0");
//     client.println("Connection: close");
//     client.println("Content-Type: application/x-www-form-urlencoded;");
//     client.print("Content-Length: ");
//     client.println(data.length());
//     Serial.println(data.length());
//     client.println();
//     client.println(data);
//
//     while(client.connected()) {
//       String line = client.readStringUntil('\n');
//       Serial.println(line);
//       if(line == "\r") {
//         Serial.println("Headers received");
//         break;
//       }
//     }
//     while(client.available()) {
//       char c = client.read();
//       Serial.write(c);
//     }
//
//     client.stop();
//   }
//
//   void heartbeat() {
//     unsigned long now = millis();
//
//     // First ever update
//     if (lastHeartBeat == 0) {
//       checkin();
//       lastHeartBeat = now;
//     }
//
//     // We have overflowed
//     if (now < lastHeartBeat) {
//       checkin();
//       lastHeartBeat = now;
//     }
//
//     // Time for an update
//     if ((now - lastHeartBeat) >= heartBeatEvery) {
//       checkin();
//       lastHeartBeat = now;
//     }
//   }
//
//   void UDPloop() {
//     int packetSize = Udp.parsePacket();
//     if (packetSize) {
//       // receive incoming UDP packets
//       Serial.printf("Received %d bytes from %s, port %d\n", packetSize, Udp.remoteIP().toString().c_str(), Udp.remotePort());
//       int len = Udp.read(incomingPacket, 255);
//       if (len > 0) {
//         incomingPacket[len] = 0;
//       }
//       Serial.printf("UDP packet contents: %s\n", incomingPacket);
//
//       StaticJsonBuffer<200> jsonBuffer;
//       JsonObject& root = jsonBuffer.parseObject(incomingPacket);
//       if (!root.success()) {
//         Serial.println("parseObject() failed");
//         return;
//       }
//
//       // Fetch values.
//       //
//       // Most of the time, you can rely on the implicit casts.
//       // In other case, you can do root["time"].as<long>();
//       double brightness = root["brightness"];
//
//       // Print values.
//       if ( DEBUG ) {
//         Serial.println(brightness*255);
//       }
//
//       ChooseBrightness(brightness*255);
//     }
//   }
//
//   void setup(void){
//     pinMode(led, OUTPUT);
//     // pinMode(RESET_WIFI_PIN, INPUT_PULLUP);
//     digitalWrite(led, LED_OFF);
//     //Serial.begin(115200); // Redundant
//
//     // See https://github.com/tzapu/WiFiManager for details, including options for triggering by pin
//     WiFiManager wifiManager; // To let the user configure which AP this connects to
//     //wifiManager.resetSettings(); //reset saved settings (trigger by pin, perhaps)
//     wifiManager.autoConnect("Soulmate Config");
//     //wifiManager.autoConnect(); //or use this for auto generated name ESP + ChipID
//
//     if (MDNS.begin("esp")) {
//       Serial.println("MDNS responder started");
//     }
//
//     server.on("/", handleRoot);
//     server.on("/list", handleListRoutines);
//     server.on("/choose", handleChooseRoutine);
//     server.on("/brightness", handleBrightness);
//     server.on("/inline", [](){
//       server.send(200, "text/plain", "this works as well");
//     });
//     server.on("/reset", handleReset);
//     server.onNotFound(handleNotFound);
//
//     server.begin();
//     Serial.println("HTTP server started");
//
//     Udp.begin(localUDPPort);
//     Serial.printf("Now listening at IP %s, UDP port %d\n", WiFi.localIP().toString().c_str(), localUDPPort);
//
//     // checkin();
//   }
//
//   void checkResetButton() {
//     // bool preserveWiFi= digitalRead(RESET_WIFI_PIN);
//     // if(!preserveWiFi) {
//     //   Serial.println("Resetting the wifi");
//     //   //Serial.println(shouldResetWiFi);
//     //   WiFi.disconnect(true);
//     //   ESP.restart();
//     // }
//   }
//
//   bool loop(void){
//     // checkResetButton();
//     server.handleClient();
//     UDPloop();
//     heartbeat();
//
//     bool didChange = changed;
//     changed = false;
//
//     return didChange;
//   }
// }
//
// #endif
//
// #endif
