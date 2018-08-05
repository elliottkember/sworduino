uint8_t maxBrightness = 1;
float brightnessScale = maxBrightness;
#include <OctoWS2811.h>
#include <FastLED.h>
#define LEDS_PER_ROW 72
#define COLS_LEDs 72  // all of the following params need to be adjusted for screen size
#define ROWS_LEDs 16 // Actually 12. Double-check this
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

bool buttonWasOff;

#define num_routines 8
int routine = 0;
const char* routines[num_routines] = {
  "Waves",
  "Sparkles",
  "Rainbow",
  "Wipe",
  "Box",
  "Squares",
  "Starfield",
  "Barbershop"
};

int bpms[864];

void setup() {
  for (int i = 0; i < 864; i++) {
    bpms[i] = random(3, 40);
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
  bool buttonIsOff = digitalRead(0);
  if (buttonIsOff && brightnessScale > 0) {
    brightnessScale -= 0.01;
  } else if (!buttonIsOff && brightnessScale < maxBrightness) {
    brightnessScale += 0.01;
  }
  if (buttonWasOff && !buttonIsOff) {
    nextRoutine();
  }
  buttonWasOff = buttonIsOff;
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

class Point {
public:
  double x, y;
};

Point drops[5] = {};
bool initializedPoints = false;

void sine() {

  if (!initializedPoints) {
    for (int i = 0; i < 5; i++) {
      Point p = drops[i];
      p.x = random(0, 16);
      p.y = random(0, 72);
      drops[i] = p;
    }
    initializedPoints = true;
  }
  
  for (int p = 0; p < 5; p++) {
    Point point = drops[p];
    point.y += 1;
    if (point.y > 30) {
      point.y = -5;
      point.x = random(0, 72);
    }
    drops[p] = point;
  }

  fadeToBlackBy(Soulmate::led_arr, N_LEDS, 40);
    
  for (int y = 12; y >= 0; y--) {
    for (int x = 0; x < 72; x++) {
      uint16_t index = y * 72 + x;

      for (int p = 0; p < 5; p++) {
        Point point = drops[p];
        double distance = sqrt16(sq(point.x - x) - sq(point.y - y));
        Serial.println(distance);
        if (distance < 1) {
          Soulmate::led_arr[index] = CHSV(p * 30, 255, 255 / distance);
        }
      }
      
//        if (y == 0) {
//          int brightness = random(0, 100) > 90 ? 255 : 0;
//          int hue = random(0, 100);
//          Soulmate::led_arr[index] = CHSV(hue, 255, brightness);
//        } else {
//          uint16_t previousIndex = (y-1) * 72 + x;
//          Soulmate::led_arr[index] = Soulmate::led_arr[previousIndex];
//        }
    }
  }
  
  map();
}

void loop() {
  checkSwitch();

  sine();

  // EVERY_N_SECONDS(300) {
  //   nextRoutine();
  // }

  // switch (routine) {
  //   case 0:
  //     waves();
  //     break;
  //   case 1:
  //     sparkles();
  //     map();
  //     break;
  //   case 2:
  //     rainbow();
  //     map();
  //     break;
  //   case 3:
  //     wipe();
  //     map();
  //     break;
  //   case 4:
  //     box();
  //     map();
  //     break;
  //   case 5:
  //     squares();
  //     map();
  //     break;
  //   case 6:
  //     starField();
  //     map();
  //     break;
  //   case 7:
  //     barbershop();
  //     map();
  //   default:
  //     break;
  // }
  
  leds.show();
}
