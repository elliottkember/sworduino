namespace Globe {
  void draw() {
    EVERY_N_MILLISECONDS(30) {
      for (int y = 0; y < 12; y++) {
        bool on = random(0, 100) > 80;
        uint8_t hue = beatsin16(20, 180, 240, y * 3000, y * 3000);
        Soulmate::led_arr[y * 72] = on ? CHSV(hue, 200, 255) : CHSV(hue, 200, 0);
        for (int x = 72; x > 0; x--) {
          uint16_t index = y * 72 + x;
          if (index < N_LEDS) {
            Soulmate::led_arr[index] = Soulmate::led_arr[index - 1];
          }
        }
      }
    }
  }
}

void globe() {
  Globe::draw();
}
