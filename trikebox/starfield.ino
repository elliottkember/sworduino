namespace StarField {
  int hue = 0;

  void draw() {
    hue = HUE_PURPLE + beatsin16(10, 0, 80);

    EVERY_N_MILLISECONDS(40) {
      // Left side
      for (int x = 36; x >= 0; x--) {
        for (int y = 0; y < 12; y++) {
          uint16_t index = y * 72 + x;

          if (x > 0) {
            Soulmate::led_arr[index] = Soulmate::led_arr[index - 1];
          } else {
            bool on = random(0, 100) > 90;
            uint8_t brightness = on ? 255 : 21;
            Soulmate::led_arr[index] = on ? CHSV(hue, 255, brightness) : CHSV(0, 0, 0);
          }
        }
      }

      // Right side
      for (int x = 36; x <= 71; x++) {
        for (int y = 0; y < 12; y++) {
          uint16_t index = y * 72 + x;

          if (x < 71) {
            Soulmate::led_arr[index] = Soulmate::led_arr[index + 1];
          } else {
            bool on = random(0, 100) > 90;
            uint8_t brightness = on ? 255 : 21;
            Soulmate::led_arr[index] = on ? CHSV(hue, 255, brightness) : CHSV(0, 0, 0);
          }
        }
      }

      // Back fade
      int fade = 60;
      for (int y = 0; y < 12; y++) {
        for (int x = 36; x <= 51; x++) {
          uint16_t index = y * 72 + x;
          Soulmate::led_arr[index].fadeToBlackBy(fade);
        }

        for (int x = 36; x >= 21; x--) {
          uint16_t index = y * 72 + x;
          Soulmate::led_arr[index].fadeToBlackBy(fade);
        }
      }
    }
  }
}

void starField() {
  StarField::draw();
}