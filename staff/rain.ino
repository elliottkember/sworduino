namespace Rain {

  int counter = 0;
  int hue = 0;

  void draw() {
    if (firstTimeRunningThroughPattern) {
      for (int i = NUM_LEDS; i > 0; i--) {
        int on = random8(100) > 80 ? 255 : 0;
        leds[i] = CHSV(hue, 255, on);
        counter++;
        if (counter == 20) {
          hue++;
          counter = 0;
        }
      }
    } else {
      for (int i = NUM_LEDS; i > frameSize; i--) {
        leds[i] = leds[i-frameSize];
      }
      for (int i = 0; i <= frameSize + 1; i++) {
        int on = random8(100) > 80 ? 255 : 0;
        leds[i] = CHSV(hue, 255, on);
        counter++;
        if (counter == 20) {
          hue--;
          counter = 0;
        }
      }
    }
  }
}

void rain() {
  Rain::draw();
}
