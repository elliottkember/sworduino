namespace ShootingGradients {
  uint8_t numberOfWorms = 4;
  uint8_t wormsFadeRate = 100; // Very low value = longer trails.
  uint8_t wormsHueIncrement = 1; // Incremental change in hue between each dot.
  uint8_t wormsStartingHue = 180; // The current hue
  uint8_t wormsBaseBeat = 2; // Higher = faster movement.

  void draw() {
    fadeToBlackBy(Global::leds, NUM_LEDS, wormsFadeRate);
    for ( int i = 0; i < numberOfWorms; i++) {
      for ( int j = 0; j < 20; j++) {
        Global::leds[beatsin16(wormsBaseBeat + i + numberOfWorms, 0, NUM_LEDS - 20) + j] = CHSV(wormsStartingHue, 180, 255);
      }
      wormsStartingHue += wormsHueIncrement;
      if (wormsStartingHue > 270) {
        wormsStartingHue = 180;
      }
    }
  }
}

void shootingGradients() {
  ShootingGradients::draw();
}
