void (*patterns[])() = {
  // dave,
  // lanternPattern,
  // nightSparkles,
  shootingGradients,
};

int fps = 30;
int patternId = 0;
int maxPatternId = sizeof( patterns ) / sizeof(patterns[0]);

void loop () {
  FastLED.setBrightness(Global::max_bright);
  Global::firstTimeRunningThroughPattern = true;
  int endTime = millis() + ROTATION_IN_MS;
  while (millis() < endTime) {
    patterns[Global::patternId]();
    Global::firstTimeRunningThroughPattern = false;
    show_at_max_brightness_for_power();
    delay(1000 / fps);
  }
  if (maxPatternId > 1) {
    Global::firstTimeRunningThroughPattern = true;
    if (++Global::patternId == maxPatternId) { Global::patternId = 0; }
  }
}
