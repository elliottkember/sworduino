namespace ShootingGradients {
  uint8_t numberOfWorms = 2;
  uint8_t wormsFadeRate = 40; // Very low value = longer trails.
  uint8_t wormsHueIncrement = 1; // Incremental change in hue between each dot.
  uint8_t wormsStartingHue = 180; // The current hue
  uint8_t wormsBaseBeat = 10; // Higher = faster movement.
  uint8_t wormLength = 10;

  void draw() {
    fadeToBlackBy(Soulmate::led_arr, N_LEDS, wormsFadeRate);
    for ( int i = 0; i < numberOfWorms; i++) {
      for ( int j = 0; j < wormLength; j++) {
        Soulmate::led_arr[beatsin16(wormsBaseBeat + i * LEDS_PER_ROW + numberOfWorms, 0, N_LEDS - wormLength) + j] += CHSV(wormsStartingHue, 180, 255);
      }
      EVERY_N_MILLISECONDS(100) {
        wormsStartingHue += wormsHueIncrement;
      }
      if (wormsStartingHue > 270) {
        wormsStartingHue = 180;
      }
    }
  }
}

bool shooting_gradients() {
  ShootingGradients::draw();
  // // Terminate randomly
  // if(Util::rand_range(0, 30) == 0){
  //   return true; // Finished with our game
  // }
  return false; // Game ongoing
}
