namespace Lantern {
  const int snakesCount = 50;
  const int circumference = LEDS_PER_ROW;
  int numberOfSparkles = 0;
  bool increasing = true;
  uint8_t hue = 50;
  
  int curve(int value, int difference, int maximum) {
    if (value < maximum && increasing) {
      value += difference;
    } else if (value > 1) {
      value -= difference;
      increasing = false;
    } else {
      increasing = true;
      value += difference;
    }
    return value;
  }
  
  uint16_t snakes[snakesCount];
  int snake = 0;
  
  void shootingStar() {
    snakes[snake] = random(7);
    snake++;
    if (snake >= snakesCount) snake = 0;
  }
  
  bool lanternPattern() {
    EVERY_N_MILLISECONDS(40) {
      for (int i = 0; i < snakesCount; i++) {
        uint16_t j = snakes[i];
        if (j > 0) {
          // Different hue for shooting stars
          Soulmate::led_arr[j] = CHSV(hue + j / 10, 240, 255);
  
          snakes[i] += circumference;
          if (snakes[i] >= N_LEDS) snakes[i] = 0;
        }
      }
    }
  
    EVERY_N_MILLISECONDS(50) {
      if (random16(N_LEDS * 0.5) < numberOfSparkles) {
        shootingStar();
      }
    }
  
    EVERY_N_MILLISECONDS(10) {
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
  
    EVERY_N_MILLISECONDS(200) {
      numberOfSparkles = curve(numberOfSparkles, 4, N_LEDS * 0.03);
    }
  
    EVERY_N_MILLISECONDS(100) {
      hue -= 1;
    }

    // Terminate randomly
    if(Util::rand_range(0,30)==0){
      return true; // Finished with our game
    }
    return false; // Game ongoing
  }
}

bool lantern() {
  return Lantern::lanternPattern();
}

