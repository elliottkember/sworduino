namespace Sparkles {
  int hue;
  void sparkles() {
    EVERY_N_MILLISECONDS(10) {
      hue++;
    }
    fadeToBlackBy(Soulmate::led_arr, N_LEDS, 10);
    int numberOfSparkles = beatsin16(12, 1, N_LEDS / 200);
    for (int i = 0; i < numberOfSparkles; i++) {
      int pos = random16(N_LEDS);
      Soulmate::led_arr[pos] = CHSV(hue + pos / 30, 240, 255);
    }
  }
}

void sparkles() {
  Sparkles::sparkles();
}

