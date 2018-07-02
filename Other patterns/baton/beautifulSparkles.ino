namespace BeautifulSparkles {
  int numberOfSparkles = 0;
  int hue = 0;

  void changeNumberOfSparkles() {
    numberOfSparkles = upAndDownBy(numberOfSparkles, 1, 80, 1);
  }

  void changeColor() {
    hue += 1;
  }

  CRGB color(int pos) {
    return CHSV(hue + (pos / 10), 240, 255);
  }

  void drawSparkles() {
    for (int i = 0; i < numberOfSparkles; i++) {
      int pos = random16(NUM_LEDS);
      Global::leds[pos] = color(pos);
    }
  }

  void draw() {
    EVERY_N_MILLISECONDS(20) {
      changeNumberOfSparkles();
      drawSparkles();
    }
    EVERY_N_MILLISECONDS(10) {
      changeColor();
    }
    fadeToBlackBy(Global::leds, NUM_LEDS, 60);
  }
}

void beautifulSparkles() {
  BeautifulSparkles::draw();
}
