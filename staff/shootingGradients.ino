namespace ShootingGradients {
  uint8_t numberOfWorms = 2;
  uint8_t wormsFadeRate = 40; // Very low value = longer trails.
  uint8_t wormsHueIncrement = 1; // Incremental change in hue between each dot.
  uint8_t wormsStartingHue = 180; // The current hue
  uint8_t wormsBaseBeat = 10; // Higher = faster movement.
  uint8_t wormLength = 10;

  void draw() {
    fadeToBlackBy(Global::leds, NUM_LEDS, wormsFadeRate);
    for ( int i = 0; i < numberOfWorms; i++) {
      for ( int j = 0; j < wormLength; j++) {
        Global::leds[beatsin16(wormsBaseBeat + i * 10 + numberOfWorms, 0, NUM_LEDS - wormLength) + j] += CHSV(wormsStartingHue, 180, 255);
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

void shootingGradients() {
  EVERY_N_MILLISECONDS(1000 / 60) {
    ShootingGradients::draw();
  }
}
