namespace PurpleSparkles {
  void draw() {
    int numberOfSparkles = beatsin16(5, 1, 5);
    for (int i = 0; i < numberOfSparkles; i++) {
      uint16_t index = random(0, N_LEDS);
      Soulmate::led_arr[index] = CRGB::Magenta;
    }
      EVERY_N_MILLISECONDS(30) {
        blur2d(Soulmate::led_arr, 72, 12, 20 / numberOfSparkles);
      }
  //  EVERY_N_MILLISECONDS(30) {
      fadeToBlackBy(Soulmate::led_arr, N_LEDS, numberOfSparkles * 5);
  //  }
  }
}

void purpleSparkles() {
  PurpleSparkles::draw();
}
