namespace Squares {
  void draw() {
    uint8_t y = 0;
    uint8_t x = 0;
    int hue = 0;

    for (int y = 0; y < 12; y+=2) {
      for (int x = 0; x < 72; x+=2) {
        uint16_t index = y * 72 + x;
        hue += 36;
        uint16_t bpm = bpms[index];
        CRGB color = CHSV(bpm * 20, 255, beatsin16(bpm, 48, 255, bpm * 10, bpm * 10));

        Soulmate::led_arr[index] = color;
        Soulmate::led_arr[index + 1] = color;
        Soulmate::led_arr[index + 72] = color;
        Soulmate::led_arr[index + 73] = color;
      }
    }
  }
}

void squares() {
  Squares::draw();
}