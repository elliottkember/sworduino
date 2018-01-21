namespace Rain {
  CRGB nextLeds[N_LEDS + LEDS_PER_ROW];
  CRGB prevLeds[N_LEDS + LEDS_PER_ROW];
  double fraction;
  int counter = 0;
  uint8_t hue = 1;
  uint16_t frameCounter;
  uint16_t lastDraw = 0;
  int frameSize = LEDS_PER_ROW;
  bool firstTimeRunningThroughPattern = true;

  void count() {
    counter++;
    if (counter == 20) {
      hue++;
      counter = 0;
    }
  }

  bool blendFrames(double maxCounter) {
    bool result = frameCounter == 0;
    frameCounter++;
    blend(prevLeds, nextLeds, Soulmate::led_arr, N_LEDS, frameCounter / maxCounter * 255.0);
    if (frameCounter == maxCounter) {
      frameCounter = 0;
      memcpy (&prevLeds, &nextLeds, sizeof(prevLeds) );
    }
    return result;
  }

  void draw() {
    if (firstTimeRunningThroughPattern) {
      firstTimeRunningThroughPattern = false;
      for (int i = N_LEDS; i > 0; i--) {
        int on = random8(100) > 80 ? 255 : 0;
        nextLeds[i] = CHSV(hue, 255, on);
        count();
      }
    } else {
      for (int i = N_LEDS; i > frameSize; i--) {
        nextLeds[i] = nextLeds[i-frameSize];
      }
      for (int i = 0; i <= frameSize + 1; i++) {
        int on = random8(100) > 80 ? 255 : 0;
        nextLeds[i] = CHSV(hue, 255, on);
        count();
      }
    }
    delay(1);
  }

  void reset() {
    firstTimeRunningThroughPattern = true;
  }
}

bool rain() {
  if (Rain::blendFrames(2.0)) {
    Rain::draw();
  }

  // Terminate randomly
  if(Util::rand_range(0,30)==0){
    return true; // Finished with our game
  }
  return false; // Game ongoing
}
