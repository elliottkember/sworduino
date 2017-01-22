namespace NightSparkles {

  int hue = 0;
  int frameSize = CIRCUMFERENCE;
  uint8_t counter = 0;

  void draw() {
    if (Global::firstTimeRunningThroughPattern) {
      for (int i = NUM_LEDS; i > 0; i--) {
        int on = random(100) > 80 ? 255 : 0;
        Global::leds[i] = CHSV(hue, 255, on);
        counter++;
        if (counter == 20) {
          hue++;
          counter = 0;
        }
      }
    } else {
      for (int i = NUM_LEDS; i > frameSize; i--) {
        Global::leds[i] = Global::leds[i-frameSize];
      }
      for (int i = 0; i <= frameSize + 1; i++) {
        int on = random(100) > 80 ? 255 : 0;
        Global::leds[i] = CHSV(hue, 255, on);
        counter++;
        if (counter == 20) {
          hue--;
          counter = 0;
        }
      }
    }
  }
}

void nightSparkles() {
  NightSparkles::draw();
}
