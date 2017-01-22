namespace Sparkles {

  int numberOfSparkles = 0;
  int hue = 0;

  void draw() {
    fadeToBlackBy(Global::leds, NUM_LEDS, 200);
    numberOfSparkles = upAndDownBy(numberOfSparkles, 1, 80, 5);
    for (int i = 0; i < numberOfSparkles * 4; i++) {
      int pos = random16(NUM_LEDS);
      Global::leds[pos] = CHSV(hue, 240, 255);
    }
    hue += 1;
  }
}

void sparkles() {
  Sparkles::draw();
}
