namespace Lantern {
  const int snakesCount = 10;
  const int circumference = LEDS_PER_ROW;
  int numberOfSparkles = 0;
  bool increasing = true;
  uint8_t hue = 50;

  uint16_t snakes[snakesCount];
  int snake = 0;

  void shootingStar() {
    snakes[snake] = random(LEDS_PER_ROW);
    snake++;
    if (snake >= snakesCount) snake = 0;
  }

  void lanternPattern() {
    numberOfSparkles = beatsin16(15, 4, N_LEDS * 0.03);

    EVERY_N_MILLISECONDS(100) {
      hue -= 1;
    }
    
    EVERY_N_MILLISECONDS(40) {
      for (int i = 0; i < snakesCount; i++) {
        uint16_t j = snakes[i];
        if (j > 0) {
          if (j > 1) {
            Soulmate::led_arr[j-1] = CHSV(hue + j / 10 + 10, 180, 100);
          }
          Soulmate::led_arr[j] = CHSV(hue + j / 10 + 10, 180, 180);
          if (j < N_LEDS - 1) {
            Soulmate::led_arr[j+1] = CHSV(hue + j / 10 + 10, 180, 100);
          }

          // angle
          snakes[i] += circumference + 1;
          if (snakes[i] >= N_LEDS) snakes[i] = 0;
        }
      }
    }

    EVERY_N_MILLISECONDS(10) {
      if (random16(N_LEDS * 0.4) < numberOfSparkles) {
        shootingStar();
      }
      fade_raw(Soulmate::led_arr, N_LEDS, 1);
    }

    EVERY_N_MILLISECONDS(60) {
      for (int i = 0; i < numberOfSparkles; i++) {
        int pos = random16(N_LEDS);
        if (!Soulmate::led_arr[pos]) {
          Soulmate::led_arr[pos] = CHSV(hue + (pos / 15), 255, 128);
        }
      }
    }
  }
}

void lantern() {
  Lantern::lanternPattern();
}

