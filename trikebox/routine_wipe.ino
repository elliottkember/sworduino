namespace WipeRoutine {
  float hue = 0;
  int col = 0;
  void draw() {
    col++;
    if (col == 72) { col = 0; }
    hue += beatsin16(5, 5, 50) / 10.0;
    for (int y = 0; y < 12; y++) {
      for (int x = 0; x < 72; x++) {
        uint16_t index = y * 72 + x;
        uint8_t brightness = 255;
        Soulmate::led_arr[index] = CHSV(hue + x*2 + y*4, 255, brightness);
      }
    }
  }
}

void wipe() {
  WipeRoutine::draw();
}