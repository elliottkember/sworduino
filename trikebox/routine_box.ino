namespace Box {
  int hue = 0;
  void draw() {
    hue += 1;
    for (int y = 0; y < 12; y++) {
      for (int x = 0; x < 72; x++) {
        uint16_t index = y * 72 + x;
        if (x < 21) {
          // side
          Soulmate::led_arr[index] = CHSV(hue + 0, 255, 255);
        } else if (x >= 72 - 21) {
          // side
          Soulmate::led_arr[index] = CHSV(hue + 85, 255, 255);
        } else {
          // back
          Soulmate::led_arr[index] = CHSV(hue + 170, 255, 255);
        }
      }
    }
  }
}

void box() {
  Box::draw();
}