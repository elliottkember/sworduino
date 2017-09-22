
CRGB nextLeds[NUM_LEDS + 10];
CRGB prevLeds[NUM_LEDS + 10];
double fraction;

namespace Rain {
  int counter = 0;
  uint8_t hue = 1;
  int frameSize = CIRCUMFERENCE;

  void count() {
//    counter++;
//    if (counter == 20) {
//      hue++;
//      counter = 0;
//    }
  }

  void draw() {
    if (Global::firstTimeRunningThroughPattern) {
      for (int i = NUM_LEDS; i > 0; i--) {
        int on = random8(100) > 80 ? 255 : 0;
        nextLeds[i] = CHSV(hue, 255, on);
        count();
      }
    } else {
      for (int i = NUM_LEDS; i > frameSize; i--) {
        nextLeds[i] = nextLeds[i-frameSize];
      }
      for (int i = 0; i <= frameSize + 1; i++) {
        int on = random8(100) > 80 ? 255 : 0;
        nextLeds[i] = CHSV(hue, 255, on);
        // Global::leds[i] = CHSV(lean * 255, 180, on);
        count();
      }
    }
  }
}

uint16_t frameCounter;
uint16_t lastDraw = 0;

void rain() {  
  if (blendFrames(80.0)) {
    Rain::draw();
  }
}

bool blendFrames(double maxCounter) {
  bool result = frameCounter == 0;
  frameCounter++;
  blend(prevLeds, nextLeds, Global::leds, NUM_LEDS, frameCounter / maxCounter * 255.0);
  if (frameCounter == maxCounter) {
    frameCounter = 0;
    memcpy (&prevLeds, &nextLeds, sizeof(prevLeds) );
  }
  return result;
}

