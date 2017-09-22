namespace BeautifulSparkles {
  int numberOfSparkles = 0;
  int hue = 0;

  void draw() {
    fadeToBlackBy(Global::leds, NUM_LEDS, 140);
    numberOfSparkles = upAndDownBy(numberOfSparkles, 1, 80, 5);
    for (int i = 0; i < numberOfSparkles * 4; i++) {
      int pos = random16(NUM_LEDS);
      Global::leds[pos] = CHSV(hue + (pos / 10), 240, 255);
    }
    hue += 10;
  }
}

void beautifulSparkles() {
  BeautifulSparkles::draw();
}
