#if defined(ESP8266)
  #define DATA_PIN 14 // Which pin data is sent to on the ESP8266 (DOUT)
//  #define CLOCK_PIN 13 // Which pin the clock is triggered on the ESP8266 (SPI)
  #define BUTTON_PIN 1 // Which pin receives signal that a button was pushed?
  #define REMOTE_CONTROL_PIN -1 //Receives data from an IR remote control
//#elif defined(ESP32)
//  #define DATA_PIN 18//23 // Which pin data is sent to on the Teensy (DOUT)
//  #define CLOCK_PIN 23//18 // Which pin the clock is triggered on the Teensy (SPI)
//  #define BUTTON_PIN -1 // Which pine receives signal that a button was pushed?
//  #define REMOTE_CONTROL_PIN -1 //Receives data from an IR remote control
//#else
////Teensy by default
//  #define DATA_PIN 11 // Which pin data is sent to on the Teensy (DOUT)
//  #define CLOCK_PIN 13 // Which pin the clock is triggered on the Teensy (SPI)
//  #define BUTTON_PIN 6 // Which pin receives signal that a button was pushed?
//  #define REMOTE_CONTROL_PIN 14 //Receives data from an IR remote control
#elif defined(ESP32)
  #define DATA_PIN 23
  #define CLOCK_PIN 18
  //#define DATA_PIN 14 // for doit esp32 board
  //#define CLOCK_PIN 15 // for doit esp32 board
  #define BUTTON_PIN -1 // Which pine receives signal that a button was pushed?
  #define REMOTE_CONTROL_PIN -1 //Receives data from an IR remote control
#else
  #define DATA_PIN 11 // Which pin data is sent to on the Teensy (DOUT)
  #define CLOCK_PIN 13 // Which pin the clock is triggered on the Teensy (SPI)
  #define BUTTON_PIN 6 // Which pin receives signal that a button was pushed?
  #define REMOTE_CONTROL_PIN 14 //Receives data from an IR remote control
#endif

//FastLED
#define N_LEDS 40 // Number of LEDs on our light strip // Max seems to be 1520 on ESP32
#define MAX_ALLOWED_VOLTS 5
#define MAX_ALLOWED_MILLIAMPS 5000
#define LED_TYPE WS2812B
#define LED_COLOR_ORDER GRB

// JPP
#define DEBUG false
#define LEDS_PER_ROW 8
#define N_CELLS (N_LEDS + N_LEDS%LEDS_PER_ROW)
#define FPS 60
#define DELAY (1000/FPS) // in milliseconds
#define RANDOMIZE_ROUTINES false // true to iterate through all available routines without requiring a button press
