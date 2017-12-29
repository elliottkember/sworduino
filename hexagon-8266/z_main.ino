#ifdef TEENSYDUINO
  #include <IRremote.h>
  #include <Entropy.h>
#endif

#if defined(ESP8266) || defined(ESP32)
  // #include "IRremoteESP8266.h"
  // #include "IRrecv.h"
  // #include "IRutils.h"
  // #include <Entropy.h>
#endif

namespace Main {
  #ifdef IRremote_h
  IRrecv irrecv(REMOTE_CONTROL_PIN); // Instruct the device to receive IR commands
  decode_results irInput; // The last input from the IR receiver
  #endif

  uint8_t brightness = 255; // From 0-255, 0 makes the device black, 255 keeps the brightness as is defined by the routine
  
  const int num_routines = 6;
  
  const char* routines[num_routines] PROGMEM = {
    // 0
    "Off",
    "Spin",
    "Fast Spin",
    "Slow Spin",
    "Slow Pulse",
    "Beautiful Sparkles",
    // 5
  };

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
//    FastLED.addLeds<LED_TYPE, DATA_PIN, CLOCK_PIN, LED_COLOR_ORDER>(Global::led_arr, N_LEDS);
    FastLED.addLeds<LED_TYPE, 5, LED_COLOR_ORDER>(Global::led_arr, N_LEDS);
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

    for(int i=0; i<N_CELLS; i++){
      Global::led_arr[i] = CRGB(80, 0, 0);
    }
    FastLED.show();

    fastled();
  }

  void reset_all() {
  }
}

void setup() {
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

void loop() {
  ESP.wdtFeed();
  bool finishedRound = false;
  int gameDelay = DELAY;
  
  if ( strcmp(Main::SafeRoutineName(Main::which_routine) , "Off") == 0) {
    finishedRound = black();
  } else if ( strcmp(Main::SafeRoutineName(Main::which_routine) , "Spin") == 0) {
    hexagon();
  } else if ( strcmp(Main::SafeRoutineName(Main::which_routine) , "Fast Spin") == 0) {
    fastHexagon();
  } else if ( strcmp(Main::SafeRoutineName(Main::which_routine) , "Beautiful Sparkles") == 0) {
    finishedRound = beautiful_sparkles();
  } else if ( strcmp(Main::SafeRoutineName(Main::which_routine) , "Slow Spin") == 0) {
    slowHexagon();
  } else if ( strcmp(Main::SafeRoutineName(Main::which_routine) , "Slow Pulse") == 0) {
    slowPulse();
  }

  if(Main::brightness < 255) {
    nscale8_video(Global::led_arr, N_LEDS, Main::brightness);
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



