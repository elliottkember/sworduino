namespace Worms {
uint8_t numberOfWorms = 4;
uint8_t wormsFadeRate = 80; // Very low value = longer trails.
uint8_t wormsHueIncrement = 3; // Incremental change in hue between each dot.
uint8_t wormsStartingHue = 180; // The current hue
uint8_t wormsBaseBeat = round(LEDS_PER_ROW/ 5); // Higher = faster movement.
  uint8_t wormLength = LEDS_PER_ROW* 5;

  void draw() {
    fadeToBlackBy(Soulmate::led_arr, N_LEDS, wormsFadeRate);
    for ( int i = 0; i < numberOfWorms; i++) {
      for ( int j = 0; j < wormLength; j++) {
        Soulmate::led_arr[beatsin16(wormsBaseBeat + i + numberOfWorms, 0, N_LEDS - wormLength) + j] += CHSV(wormsStartingHue, 180, 255);
      }
      if (i % 2 == 0) {
        wormsStartingHue += wormsHueIncrement;
      }
      if (wormsStartingHue > 270) {
        wormsStartingHue = 180;
      }
    }
  }
}

bool worms() {
  Worms::draw();

  // Terminate randomly
  if(Util::rand_range(0,30)==0){
    return true; // Finished with our game
  }
  return false; // Game ongoing
}

