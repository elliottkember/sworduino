#define FASTLED_ALLOW_INTERRUPTS 0
#define FASTLED_ESP8266_NODEMCU_PIN_ORDER
#define MAX_ALLOWED_VOLTS 5
#define FPS 60
#define DELAY (1000/FPS) // in milliseconds
#define DEBUG false
#define RANDOMIZE_ROUTINES false // true to iterate through all available routines without requiring a button press

#if defined(ESP8266)
  #define DATA_PIN 5 // Which pin data is sent to on the ESP8266 (DOUT)
  #define CLOCK_PIN 13 // Which pin the clock is triggered on the ESP8266 (SPI)
  #define BUTTON_PIN 3 // Which pin receives signal that a button was pushed?
  #define REMOTE_CONTROL_PIN -1 //Receives data from an IR remote control
#elif defined(ESP32)
 #define DATA_PIN 18//23 // Which pin data is sent to on the Teensy (DOUT)
 #define CLOCK_PIN 23//18 // Which pin the clock is triggered on the Teensy (SPI)
 #define BUTTON_PIN -1 // Which pine receives signal that a button was pushed?
 #define REMOTE_CONTROL_PIN -1 //Receives data from an IR remote control
#else
  //Teensy by default
 #define DATA_PIN 11 // Which pin data is sent to on the Teensy (DOUT)
 #define CLOCK_PIN 13 // Which pin the clock is triggered on the Teensy (SPI)
 #define BUTTON_PIN 6 // Which pin receives signal that a button was pushed?
 #define REMOTE_CONTROL_PIN 14 //Receives data from an IR remote control
#endif

namespace Util {
  int rand_range(int min, int max) {
    return random(min, (max+1));
  }
}

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
  const unsigned long heartBeatEvery = 7200000; // Checks in with the server every N milliseconds
  unsigned long lastHeartBeat; // Millisecond timestamp at time of last heartbeat
  //WiFiClient client;
  WiFiClientSecure client;

  // UDP
  WiFiUDP Udp;
  unsigned int localUDPPort = 12345;
  char incomingPacket[255];

  bool changed = false;

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
    message += "\"" + String(routines[i]) + "\" ] }";
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

  bool loop(void){
    // checkResetButton();
    server.handleClient();
    UDPloop();
    heartbeat();

    bool didChange = changed;
    changed = false;

    return didChange;
  }
}

#endif

#endif


#ifdef TEENSYDUINO
  // #include <IRremote.h>
  // #include <Entropy.h>
#endif

#if defined(ESP8266) || defined(ESP32)
  // #include "IRremoteESP8266.h"
  // #include "IRrecv.h"
  // #include "IRutils.h"
  // #include <Entropy.h>
#endif

namespace Main {

  uint8_t brightness = 255;

  #ifdef IRremote_h
  IRrecv irrecv(REMOTE_CONTROL_PIN); // Instruct the device to receive IR commands
  decode_results irInput; // The last input from the IR receiver
  #endif

  int SafeRoutineID(int i) {
    if (i >= num_routines) {
      return 0;
    }

    return i;
  }

  const char* SafeRoutineName(int i) {
    return routines[SafeRoutineID(i)];
  }

  int which_routine;
  bool buttonValue = true;

  int pick_routine() {
    return Util::rand_range(0,num_routines-1);
  }

  void wemos() {
    ESP.wdtDisable();
    Serial.begin(115200);
    //    pinMode(DATA_PIN, OUTPUT);
    //    pinMode(BUTTON_PIN, INPUT_PULLUP);
    //    digitalWrite(DATA_PIN, HIGH); // Initialize all LEDs to be off by default
    #if defined(ESP8266) || defined(ESP32)
    Wifi::setup();
    #endif

    which_routine = pick_routine();
  }

  void teensy() {
    Serial.begin(31250); // No error on https://www.pjrc.com/teensy/td_uart.html

    // Configure the Teensy based on the output pin we have chosen for the LED strip
    pinMode(DATA_PIN, OUTPUT);
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    pinMode(REMOTE_CONTROL_PIN, INPUT_PULLUP);
    digitalWrite(DATA_PIN, HIGH); // Initialize all LEDs to be off by default

    #ifdef IRremote_h
    Serial.println("Enabling IRin");
    irrecv.enableIRIn(); // Start the IR receiver
    Serial.println("Enabled IRin");
    #endif

    #ifdef Entropy_h
    // Make the data random each time by seeding with random analogue data
    Entropy.Initialize();
    randomSeed(Entropy.random());
    #endif

    which_routine = pick_routine();
  }

  void fastled() {
    // Tell FastLED we are using an APA102 LED strand
    // Define the data and clock pins here to use hardware SPI
    // as per https://github.com/FastLED/FastLED/wiki/SPI-Hardware-or-Bit-banging

    // LED SETUP LINES

    // FastLED.addLeds<LED_TYPE, DATA_PIN, CLOCK_PIN, LED_COLOR_ORDER>(Global::led_arr, N_LEDS);
    FastLED.addLeds<LED_TYPE, DATA_PIN, LED_COLOR_ORDER>(Global::led_arr, N_LEDS);
    FastLED.setMaxPowerInVoltsAndMilliamps(MAX_ALLOWED_VOLTS, MAX_ALLOWED_MILLIAMPS);
    FastLED.setCorrection(ClearBlueSky);
  }

  void setup() {
    #if defined(ESP8266) || defined(ESP32)
      wemos();
    #else
      // Default is to assume TEENSYDUINO compatibility
      teensy();
    #endif

    for(int i=0; i<N_LEDS; i++){
      Global::led_arr[i] = CRGB(80, 0, 0);
    }
    FastLED.show();

    fastled();
  }

  void reset_all() {
  }
}

void soulmateSetup() {
  wdt_disable();
  pinMode(13, OUTPUT);
  Main::setup();
  #if defined(ESP8266)
  Serial.println("THIS IS AN ESP8266");
  #endif
  #if defined(ESP32)
  Serial.println("THS IS AN ESP 32");
  #endif
  Serial.println('End setup');
}

void soulmateLoop() {
  ESP.wdtFeed();
  bool finishedRound = false;
  int gameDelay = DELAY;

  playPattern(Main::which_routine);

  if(Main::brightness < 255) {
    FastLED.setBrightness(Main::brightness);
  }

  FastLED.show();

  // Keep the LED display unchanged for gameDelay() milliseconds, but permit button interrupts
  unsigned long currentMillis = millis();
  for(;;) {
    // Detect button press
    bool newButtonValue = digitalRead(BUTTON_PIN);
    bool changeRoutine = false;

    // Detect button press via Wifi
    #if defined(ESP8266) || defined(ESP32)
    bool wifiChanged = Wifi::loop();
    if(wifiChanged) {
      // Mimic the trigger phase
      Main::buttonValue = true;
      newButtonValue = false;
    }
    #endif

    // For now, any input on the remote will just be treated as a button press
    #ifdef IRremote_h
    decode_results results;
    if (Main::irrecv.decode(&results)) {
      // The device often sends duplicate input. 0xffffffffu is code for "repeat the last command"
      if(results.value != 0xffffffffu){

        if(DEBUG) {
          Serial.println(results.value, HEX);
          Serial.println(results.decode_type);
          Serial.println(millis());
        }

        Main::irInput = results;

        // Mimic the trigger phase
        Main::buttonValue = true;
        newButtonValue = false;
      }
      Main::irrecv.resume(); // Receive the next value
    }
    #endif

    // React to button press
    if(Main::buttonValue != newButtonValue){
      if(DEBUG) {
        Serial.printf("Button value now %d, was %d.\n", newButtonValue, Main::buttonValue);
      }
      Main::buttonValue = newButtonValue;

      // Act on the button press only during the "trigger" phase of the button (which is "off")
      if(!newButtonValue) {
        Main::which_routine = (Main::which_routine+1)%Main::num_routines;
        finishedRound = true;

        // Reset stateful games
        Main::reset_all();

        // Notify the delay function not to delay any longer
        changeRoutine = true;
      }
    }

    // Then, wake up, check to see if there is a button press or an IR signal
    // Apparently this is no more processor intensive on an arduino than delay()
    // but it's better because it allows interrupts.
    if(changeRoutine || ((millis() - currentMillis) >= (unsigned long)gameDelay)) {
      break;
    }
  }

  // If our routine finished, we can choose another one:
  if(finishedRound && RANDOMIZE_ROUTINES){
    Main::which_routine = Main::pick_routine();

    if(DEBUG){
      Serial.println(Main::which_routine);
    }
  }
}

// TODO: Should we remove all these?

const char* ChooseRoutine(int i) {
  Main::which_routine = Main::SafeRoutineID(i);
  Main::reset_all();

  return Main::SafeRoutineName(Main::which_routine);
}

String CurrentRoutine() {
  return String(Main::which_routine);
}

const char** Routines() {
  return Main::routines;
}

const int NumRoutines() {
  return Main::num_routines;
}

uint8_t CurrentBrightness() {
  return Main::brightness;
}

void ChooseBrightness(uint8_t brightness) {
  Main::brightness = brightness;
}
