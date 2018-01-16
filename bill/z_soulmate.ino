// Soulmate config stuff

#define FASTLED_ALLOW_INTERRUPTS 0
#define FASTLED_ESP8266_NODEMCU_PIN_ORDER
#define MAX_ALLOWED_VOLTS 5
#define FPS 60
#define DELAY (1000/FPS) // in milliseconds
#define DEBUG false

#if defined(ESP8266)
  #define DATA_PIN 5 // Which pin data is sent to on the ESP8266 (DOUT)
  #define CLOCK_PIN 7 // Which pin the clock is triggered on the ESP8266 (SPI)
  #define BUTTON_PIN 3 // Which pin receives signal that a button was pushed?
  #define REMOTE_CONTROL_PIN -1 //Receives data from an IR remote control
#elif defined(ESP32)
 #define DATA_PIN 18 //23 // Which pin data is sent to on the Teensy (DOUT)
 #define CLOCK_PIN 23 //18 // Which pin the clock is triggered on the Teensy (SPI)
 #define BUTTON_PIN -1 // Which pine receives signal that a button was pushed?
 #define REMOTE_CONTROL_PIN -1 //Receives data from an IR remote control
#else // Teensy
 #define DATA_PIN 11 // Which pin data is sent to on the Teensy (DOUT)
 #define CLOCK_PIN 13 // Which pin the clock is triggered on the Teensy (SPI)
 #define BUTTON_PIN 6 // Which pin receives signal that a button was pushed?
 #define REMOTE_CONTROL_PIN 14 //Receives data from an IR remote control
#endif





// Wifi config


#ifndef LIB_WIFI
#define LIB_WIFI

#if defined(ESP32) || defined(ESP8266)
#if defined(ESP32)
  #include <WiFi.h>
  #include <WebServer.h>
  #include <ESPmDNS.h>
#elif defined(ESP8266)
  #include <ESP8266WiFi.h>
  #include <ESP8266WebServer.h>
  #include <ESP8266mDNS.h>
#endif

#include <DNSServer.h>
#include <WiFiClientSecure.h>
#include <WiFiManager.h>

#if defined(ESP32)
  WebServer server(80);
#elif defined(ESP8266)
  ESP8266WebServer server(80);
#endif

#include <WiFiUdp.h>
#include <ArduinoJson.h>

namespace Wifi {
  const int LED_ON = LOW;
  const int LED_OFF = HIGH;
  const int led = BUILTIN_LED;
  const unsigned long heartBeatEvery = 7200000; // ms
  unsigned long lastHeartBeat;
  WiFiClientSecure client;
  WiFiUDP Udp;
  unsigned int localUDPPort = 12345;
  char incomingPacket[255];

    void handleNotFound(){
    digitalWrite(led, LED_ON);
    String message = "File Not Found\n\n";
    message += "URI: ";
    message += server.uri();
    message += "\nMethod: ";
    message += (server.method() == HTTP_GET)?"GET":"POST";
    message += "\nArguments: ";
    message += server.args();
    message += "\n";
    for (uint8_t i=0; i<server.args(); i++){
      message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
    }
    server.send(404, "text/plain", message);
    digitalWrite(led, LED_OFF);
  }

  void handleRoot() {
    digitalWrite(led, LED_ON);
    server.send(200, "text/plain", String("hello from esp with MAC " + WiFi.macAddress() + "! Running routine #" + CurrentRoutine()));
    digitalWrite(led, LED_OFF);
    black();
  }

  void handleChooseRoutine() {
    if (server.arg("routine") == "") {
      // server.send(404, "text/plain", "Not found");
      handleNotFound();
      return;
    }

    String routine = server.arg("routine");
    const char* chosen = ChooseRoutine(routine.toInt());
    const char* name = CurrentRoutineName();
    server.send(200, "text/plain", String("Selected ") + chosen);
  }

  void handleBrightness() {
    String brightness_submitted = server.arg("brightness");
    if(brightness_submitted.length() > 0) {
      ChooseBrightness((uint8_t)brightness_submitted.toInt());
    }

    String brightness = String(CurrentBrightness());
    server.send(200, "text/plain", brightness);
  }

  void handleListRoutines() {
    String message = "{ \"CurrentRoutine\": " + CurrentRoutine() + ", \"Routines\": [ ";
    const char** routines = Routines();
    int i = 0;
    for (;i < NumRoutines()-1; i++) {
      message += "\"" + String(routines[i]) + "\", ";
    }
    // (Note that i was already incremented by the loop, no need to increment again)
    message += "\"" + String(routines[i]) + "\" ]";

    message += ", \"on\": ";
    message += IsOn() ? "true" : "false";

    message += ", \"brightness\": ";
    message += CurrentBrightness();

    #if defined(LEDS_WS2812B)
      message += ", \"strip\": \"WS2812B\"";
    #else
      message += ", \"strip\": \"APA102\"";
    #endif

    message += "}";
    server.send(200, "application/json", message);
  }

  void handleReset() {
    WiFi.disconnect(true);
    delay(1000);
    ESP.restart();
  }

  void checkin() {
    String data = String("firmware=1.0")
      + String("&lan_ip=") + String(WiFi.localIP()[0]) + "." + String(WiFi.localIP()[1]) + "." + String(WiFi.localIP()[2]) + "." + String(WiFi.localIP()[3])
      + String("&mac=") + String(WiFi.macAddress());

    //IPAddress host = IPAddress(192,168,1,47);
    //uint16_t port = 9019;
    const char* host = "soulmate.carbocation.com";
    uint16_t port = 443;

    if (!client.connect(host, port)) {
      Serial.println("checkin failed");
      return;
    }

    client.println("POST /soulmate/v1/checkin HTTP/1.1");
    client.println(String("Host: ") + String(host));
    client.println("User-Agent: Soulmate/1.0");
    client.println("Connection: close");
    client.println("Content-Type: application/x-www-form-urlencoded;");
    client.print("Content-Length: ");
    client.println(data.length());
    Serial.println(data.length());
    client.println();
    client.println(data);

    while(client.connected()) {
      String line = client.readStringUntil('\n');
      Serial.println(line);
      if(line == "\r") {
        Serial.println("Headers received");
        break;
      }
    }
    while(client.available()) {
      char c = client.read();
      Serial.write(c);
    }

    client.stop();
  }

  void heartbeat() {
    unsigned long now = millis();

    // First ever update
    if (lastHeartBeat == 0) {
      checkin();
      lastHeartBeat = now;
    }

    // We have overflowed
    if (now < lastHeartBeat) {
      checkin();
      lastHeartBeat = now;
    }

    // Time for an update
    if ((now - lastHeartBeat) >= heartBeatEvery) {
      checkin();
      lastHeartBeat = now;
    }
  }

  void UDPloop() {
    int packetSize = Udp.parsePacket();
    if (packetSize) {
      // receive incoming UDP packets
      Serial.printf("Received %d bytes from %s, port %d\n", packetSize, Udp.remoteIP().toString().c_str(), Udp.remotePort());
      int len = Udp.read(incomingPacket, 255);
      if (len > 0) {
        incomingPacket[len] = 0;
      }
      Serial.printf("UDP packet contents: %s\n", incomingPacket);

      StaticJsonBuffer<200> jsonBuffer;
      JsonObject& root = jsonBuffer.parseObject(incomingPacket);
      if (!root.success()) {
        Serial.println("parseObject() failed");
        return;
      }

      // Fetch values.
      //
      // Most of the time, you can rely on the implicit casts.
      // In other case, you can do root["time"].as<long>();
      double brightness = root["brightness"];

      // Print values.
      if ( DEBUG ) {
        Serial.println(brightness*255);
      }

      ChooseBrightness(brightness*255);
    }
  }

  void setup(void){
    pinMode(led, OUTPUT);
    // pinMode(RESET_WIFI_PIN, INPUT_PULLUP);
    digitalWrite(led, LED_OFF);
    //Serial.begin(115200); // Redundant

    // See https://github.com/tzapu/WiFiManager for details, including options for triggering by pin
    WiFiManager wifiManager; // To let the user configure which AP this connects to
    //wifiManager.resetSettings(); //reset saved settings (trigger by pin, perhaps)
    wifiManager.autoConnect("Soulmate Config");
    //wifiManager.autoConnect(); //or use this for auto generated name ESP + ChipID

    if (MDNS.begin("esp")) {
      Serial.println("MDNS responder started");
    }

    server.on("/", handleRoot);
    server.on("/list", handleListRoutines);
    server.on("/choose", handleChooseRoutine);
    server.on("/brightness", handleBrightness);
    server.on("/inline", [](){
      server.send(200, "text/plain", "this works as well");
    });
    server.on("/reset", handleReset);
    server.onNotFound(handleNotFound);

    // Homekit
    server.on("/on", [](){
      TurnOn();
      server.send(200, "text/plain", "on");
    });
    server.on("/off", [](){
      TurnOff();
      server.send(200, "text/plain", "off");
    });

    server.begin();
    Serial.println("HTTP server started");

    Udp.begin(localUDPPort);
    Serial.printf("Now listening at IP %s, UDP port %d\n", WiFi.localIP().toString().c_str(), localUDPPort);

    // checkin();
  }

  void checkResetButton() {
    // bool preserveWiFi= digitalRead(RESET_WIFI_PIN);
    // if(!preserveWiFi) {
    //   Serial.println("Resetting the wifi");
    //   //Serial.println(shouldResetWiFi);
    //   WiFi.disconnect(true);
    //   ESP.restart();
    // }
  }

  void loop(){
    // checkResetButton();
    server.handleClient();
    UDPloop();
    heartbeat();
  }
}

#endif

#endif





// Main Soulmate class

namespace Soulmate {
  uint8_t brightness = 255;
  int which_routine = 0;
  bool buttonValue = true;
  bool on = true;

  int SafeRoutineID(int i) {
    return i >= num_routines ? 0 : i;
  }

  const char* SafeRoutineName(int i) {
    return routines[SafeRoutineID(i)];
  }

  void wemos() {
    ESP.wdtDisable();
    Serial.begin(115200);
    // pinMode(DATA_PIN, OUTPUT);
    // pinMode(BUTTON_PIN, INPUT_PULLUP);
    // digitalWrite(DATA_PIN, HIGH); // Initialize all LEDs to be off by default
    #if defined(ESP8266) || defined(ESP32)
    Wifi::setup();
    #endif
  }

  void teensy() {
    Serial.begin(31250); // No error on https://www.pjrc.com/teensy/td_uart.html
    pinMode(DATA_PIN, OUTPUT);
    digitalWrite(DATA_PIN, HIGH);
  }

  void fastled() {
    // Tell FastLED we are using an APA102 LED strand
    // Define the data and clock pins here to use hardware SPI
    // as per https://github.com/FastLED/FastLED/wiki/SPI-Hardware-or-Bit-banging

    // LED SETUP LINES

    #if defined(LEDS_WS2812B)
      FastLED.addLeds<LED_TYPE, DATA_PIN, GRB>(Soulmate::led_arr, N_LEDS);
    #else
      FastLED.addLeds<LED_TYPE, DATA_PIN, CLOCK_PIN, BGR>(Soulmate::led_arr, N_LEDS);
    #endif
    FastLED.setMaxPowerInVoltsAndMilliamps(MAX_ALLOWED_VOLTS, MAX_ALLOWED_MILLIAMPS);
    FastLED.setCorrection(ClearBlueSky);
  }

  void setup() {
    wdt_disable();
    pinMode(13, OUTPUT);

    #if defined(ESP8266) || defined(ESP32)
      wemos();
    #else
      // Default is to assume TEENSYDUINO compatibility
      teensy();
    #endif

    for(int i=0; i<N_LEDS; i++){
      Soulmate::led_arr[i] = CRGB(80, 0, 0);
    }
    FastLED.show();

    fastled();
  }

  void loop() {
    ESP.wdtFeed();
    bool finishedRound = false;
    int gameDelay = DELAY;

    if (on) {
      playPattern(Soulmate::which_routine);
      if(Soulmate::brightness <= 255) {
        FastLED.setBrightness(Soulmate::brightness);
      }
    } else {
      FastLED.setBrightness(0);
    }

    FastLED.show();

    // Keep the LED display unchanged for gameDelay() milliseconds, but permit button interrupts
    unsigned long currentMillis = millis();
    for(;;) {
      #if defined(ESP8266) || defined(ESP32)
      Wifi::loop();
      #endif

      // Then, wake up, check to see if there is a button press or an IR signal
      // Apparently this is no more processor intensive on an arduino than delay()
      // but it's better because it allows interrupts.
      if(((millis() - currentMillis) >= (unsigned long)gameDelay)) {
        break;
      }
    }
  }
}




// The main accessor functions for using Soulmate

void soulmateSetup() {
  Soulmate::setup();
}

void soulmateLoop() {
  Soulmate::loop();
}




// Accessors for cross-class communication (API)

const char* ChooseRoutine(int i) {
  Soulmate::which_routine = Soulmate::SafeRoutineID(i);

  return Soulmate::SafeRoutineName(Soulmate::which_routine);
}

String CurrentRoutine() {
  return String(Soulmate::which_routine);
}

const char* CurrentRoutineName() {
  return Soulmate::SafeRoutineName(Soulmate::which_routine);
}

const char** Routines() {
  return Soulmate::routines;
}

const int NumRoutines() {
  return Soulmate::num_routines;
}

uint8_t CurrentBrightness() {
  return Soulmate::brightness;
}

void ChooseBrightness(uint8_t brightness) {
  Soulmate::brightness = brightness;
}

bool IsOn() {
  return Soulmate::on == true;
}
void TurnOn() {
  Soulmate::on = true;
}
void TurnOff() {
  Soulmate::on = false;
}



// Sample pattern

// namespace Solid {
//   bool solid(uint8_t hue, uint8_t saturation, uint8_t value) {
//     for(int i=0; i<N_CELLS; i++){
//       Soulmate::led_arr[i] = CHSV(hue, saturation, value);
//     }
//
//     return true;
//   }
// }
//
// bool black() {
//   return Solid::solid(0, 0, 0);
// }
