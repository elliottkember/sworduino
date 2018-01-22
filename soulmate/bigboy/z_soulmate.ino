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
 #define DATA_PIN 23 //23 // Which pin data is sent to on the Teensy (DOUT)
 #define CLOCK_PIN 18 //18 // Which pin the clock is triggered on the Teensy (SPI)
 #define BUTTON_PIN -1 // Which pine receives signal that a button was pushed?
 #define REMOTE_CONTROL_PIN -1 //Receives data from an IR remote control
#else // Teensy
 #define DATA_PIN 11 // Which pin data is sent to on the Teensy (DOUT)
 #define CLOCK_PIN 13 // Which pin the clock is triggered on the Teensy (SPI)
 #define BUTTON_PIN 6 // Which pin receives signal that a button was pushed?
 #define REMOTE_CONTROL_PIN 14 //Receives data from an IR remote control
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
    Serial.begin(115200);
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
    #if defined(LEDS_WS2812B)
      FastLED.addLeds<LED_TYPE, DATA_PIN, GRB>(Soulmate::led_arr, N_LEDS);
    #else
      FastLED.addLeds<LED_TYPE, DATA_PIN, CLOCK_PIN, BGR, DATA_RATE_MHZ(1)>(Soulmate::led_arr, N_LEDS);
    #endif
    FastLED.setMaxPowerInVoltsAndMilliamps(MAX_ALLOWED_VOLTS, MAX_ALLOWED_MILLIAMPS);
  }

  void setup() {
    // wdt_disable();
    pinMode(13, OUTPUT);

    #if defined(ESP8266) || defined(ESP32)
      wemos();
    #else
      // Default is to assume TEENSYDUINO compatibility
      teensy();
    #endif

    fastled();
  }

  void loop() {
    // ESP.wdtFeed();
    bool finishedRound = false;
    int gameDelay = DELAY;

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

    if (on) {
      playPattern(Soulmate::which_routine);

      // Slowly adjust brightness
      if(Soulmate::brightness <= 255 && FastLED.getBrightness() != Soulmate::brightness) {
        if (FastLED.getBrightness() < Soulmate::brightness) {
          FastLED.setBrightness(FastLED.getBrightness() + 1);
        } else {
          FastLED.setBrightness(FastLED.getBrightness() - 1);
        }
      }
    } else {
      FastLED.setBrightness(0);
    }

    FastLED.show();
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

namespace Solid {

  void solidColor(CRGB color) {
    for(int i=0; i<N_CELLS; i++){
      Soulmate::led_arr[i] = color;
    }
  }

  void solid(uint8_t hue, uint8_t saturation, uint8_t value) {
    solidColor(CHSV(hue, saturation, value));
  }
}

void black() {
  Solid::solid(0, 0, 0);
}
