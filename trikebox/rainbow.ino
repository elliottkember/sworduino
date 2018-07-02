namespace Rainbow {
  int hue = 0;
  void draw() {
    for (uint16_t row = 0; row < ROWS_LEDs; row++) {
      uint16_t index = row * COLS_LEDs;
      int hueIndex = hue + row * beatsin16(5, -30, 30);
      fill_gradient(
        Soulmate::led_arr, 
        index, 
        CHSV(hueIndex, 255, 255), 
        index + COLS_LEDs, 
        CHSV(hueIndex + 128, 255, 255)
      );
      EVERY_N_MILLISECONDS(20) {
        hue += beatsin16(3, -5, 5);
      }
    }
  }
}

void rainbow() {
  Rainbow::draw();
}

