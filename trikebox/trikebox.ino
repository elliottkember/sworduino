float brightnessScale = 0.8;
#include <OctoWS2811.h>

//OctoWS2811 Defn. Stuff
#define COLS_LEDs 72  // all of the following params need to be adjusted for screen size
#define ROWS_LEDs 16 // LED_LAYOUT assumed 0 if ROWS_LEDs > 8
#define LEDS_PER_STRIP (COLS_LEDs * (ROWS_LEDs / 8))
#define LED_LAYOUT 0

DMAMEM int displayMemory[LEDS_PER_STRIP*6];
int drawingMemory[LEDS_PER_STRIP*6];
const int config = WS2811_GRB | WS2811_800kHz;
OctoWS2811 leds(LEDS_PER_STRIP, displayMemory, drawingMemory, config);

bool lastSwitchPosition;

void setup()
{
  Serial.begin(57600);
  pinMode(13, OUTPUT);
  pinMode(0, INPUT_PULLUP);
  leds.begin();
  leds.show();
  delay(100);
}

int routine = 0;
const int routines[num_routines] = {
  "Waves"
};

// Power switch - turns brightnessScale between 0 and 1
void checkSwitch() {
  bool off = digitalRead(0);
  if (off && brightnessScale > 0) {
    brightnessScale -= 0.01;
  } else if (!off && brightnessScale < 0.8) {
    brightnessScale += 0.01;
  }
  if (!off && lastSwitchPosition == true) {
    // Do something?
  }
  lastSwitchPosition = off;
}


void loop() {
  checkSwitch();

  switch (routines) {
    case 0:
      waves();
      break;
    default:
      waves();
      break;
  }
}
