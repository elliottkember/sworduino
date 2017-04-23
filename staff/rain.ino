namespace Rain {
  int counter = 0;
  uint8_t hue = 1;
  int frameSize = CIRCUMFERENCE;

  void count() {
    counter++;
    if (counter == 20) {
      hue++;
      counter = 0;
    }
  }

  void draw() {    
    if (Global::firstTimeRunningThroughPattern) {
      for (int i = NUM_LEDS; i > 0; i--) {
        int on = random8(100) > 80 ? 255 : 0;
        Global::leds[i] = CHSV(hue, 255, on);
        count();
      }
    } else {
      for (int i = NUM_LEDS; i > frameSize; i--) {
        Global::leds[i] = Global::leds[i-frameSize];
        if (Global::leds[i].getAverageLight() > 0) {
          Global::leds[i] = CHSV(lean * 255, 180, 255);
        }
      }
      for (int i = 0; i <= frameSize + 1; i++) {
        int on = random8(100) > 80 ? 255 : 0;
//        Global::leds[i] = CHSV(hue, 255, on);
        Global::leds[i] = CHSV(lean * 255, 180, on);
        count();
      }
    }
  }
}

void rain() {
  Rain::draw();
}
