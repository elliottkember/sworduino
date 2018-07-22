uint8_t maxBrightness = 1;
float brightnessScale = maxBrightness;
#include <OctoWS2811.h>
#include <FastLED.h>
#define LEDS_PER_ROW 72
#define COLS_LEDs 72  // all of the following params need to be adjusted for screen size
// (we actually use 12 rows. Don't know why this is 16)
#define ROWS_LEDs 16 // LED_LAYOUT assumed 0 if ROWS_LEDs > 8
#define LEDS_PER_STRIP (COLS_LEDs * (ROWS_LEDs / 8))
#define LED_LAYOUT 0

// Digital unsigned subtraction macro. if result <0, then => 0. Otherwise, take on fixed value.
#define qsubd(x, b) ((x>b)?255:0)
// Analog Unsigned subtraction macro. if result <0, then => 0
#define qsuba(x, b) ((x>b)?x-b:0)

DMAMEM int displayMemory[LEDS_PER_STRIP*6];
int drawingMemory[LEDS_PER_STRIP*6];
const int config = WS2811_GRB | WS2811_800kHz;
OctoWS2811 leds(LEDS_PER_STRIP, displayMemory, drawingMemory, config);

namespace Util {
  int rand_range(int min, int max) {
    return random(min, (max + 1));
  }
}

#define N_CELLS 1224
#define N_LEDS 1224

bool wasOff;

#define num_routines 6
int routine = 0;
const char* routines[num_routines] = {
  "Waves",
  "Sparkles",
  "Rainbow",
  "Wipe",
  "Box",
  "Squares",
};

int bpms[864];

void setup() {
  for (int i = 0; i < 864; i++) {
    bpms[i] = random(10, 30);
  }
  Serial.begin(57600);
  pinMode(13, OUTPUT);
  pinMode(0, INPUT_PULLUP);
  leds.begin();
  leds.show();
  delay(100);

  routine = random(0, num_routines);

  // Startup brightness
  brightnessScale = digitalRead(0) ? 0 : maxBrightness;
}

void nextRoutine() {
  routine += 1;
  if (routine == num_routines) routine = 0;
}

// Power switch - turns brightnessScale between 0 and 1
void checkSwitch() {
  bool off = digitalRead(0);
  if (off && brightnessScale > 0) {
    brightnessScale -= 0.01;
  } else if (!off && brightnessScale < maxBrightness) {
    brightnessScale += 0.01;
  }
  if (wasOff && !off) {
    nextRoutine();
  }
  wasOff = off;
}

namespace Soulmate {
  CRGB led_arr[N_LEDS];
}

void setPixel(uint16_t index, CRGB pixel) {
  int g = max(pixel.g * brightnessScale, 0);
  int b = max(pixel.b * brightnessScale, 0);
  int r = max(pixel.r * brightnessScale, 0);
  leds.setPixel(index, ((r << 16) | (g << 8) | b));
}

void map() {
  for (int row = 0; row < ROWS_LEDs; row++) {
    for (int col = 0; col < COLS_LEDs; col++) {
      uint16_t index = row * COLS_LEDs + col;
      if (row % 2 == 0) {
        uint16_t myIndex = row * COLS_LEDs - col - 1 + COLS_LEDs;
        setPixel(index, Soulmate::led_arr[myIndex]);
      } else {
        setPixel(index, Soulmate::led_arr[index]);
      }
    }
  }
}

void loop() {
  checkSwitch();

  EVERY_N_SECONDS(300) {
    nextRoutine();
  }

  switch (routine) {
    case 0:
      waves();
      break;
    case 1:
      sparkles();
      map();
      break;
    case 2:
      rainbow();
      map();
      break;
    case 3:
      wipe();
      map();
    case 4:
      box();
      map();
    case 5:
      squares();
      map();
    default:
      break;
  }
  
  leds.show();
}
