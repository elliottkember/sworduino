namespace BeautifulSparkles {
  int hue = 50;                 // Starting hue.
  bool firstTimeRunningThroughPattern = true;
  int numberOfSparkles = 1;
  bool increasing = true;

  int upAndDownBy(int value, int difference) {
    if (value < 20 && increasing) {
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

  bool beautifulSparkles() {
  fadeToBlackBy(Soulmate::led_arr, N_LEDS, 150);
    numberOfSparkles = upAndDownBy(numberOfSparkles, 1);
    for (int i = 0; i < numberOfSparkles * 3; i++) {
      int pos = random16(N_LEDS);
      Soulmate::led_arr[pos] = CHSV(hue + (pos / 7), 240, 255);
    }
    hue += 10;
    delay(DELAY+ 30);

    // Terminate randomly
    if(Util::rand_range(0,30)==0){
      return true; // Finished with our game
    }
    return false; // Game ongoing
  }
}

bool beautiful_sparkles() {
  return BeautifulSparkles::beautifulSparkles();
}
