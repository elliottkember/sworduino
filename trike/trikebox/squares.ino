namespace Squares {
  int hue = 0;
  void draw(int xStep, int yStep) {
    for (int y = 0; y < 12; y+=yStep) {
      for (int x = 0; x < 72; x+=xStep) {

        uint16_t index = y * 72 + x;
        hue += 36;
        uint16_t bpm = bpms[index];
        CRGB color = CHSV(bpm * 20, 255, beatsin16(bpm, 48, 255, bpm * 10, bpm * 10));

        for (int i = 0; i <= xStep; i += 1) {
          for (int j = 0; j <= yStep; j += 1) {
            Soulmate::led_arr[index + i + 72 * j] = color;
          }
        }
      }
    }
  }
}

void squares() {
  Squares::draw(4, 3);
}