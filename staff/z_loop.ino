void (*patterns[])() = {
//  solid,
//  beautifulSparkles,
//  discoBarber1,
//  discoBarber2,
//  rain,
//  sparkles,
  dave,
//  shootingGradients,
//  discoTwirl,
//  discoTwirl2,
};

int patternId = 0;
int maxPatternId = sizeof( patterns ) / sizeof(patterns[0]);

void setup() {
  randomSeed(analogRead(0));
  Serial.begin(57600);
  LEDS.addLeds<LED_TYPE, LED_DT, COLOR_ORDER>(Global::leds, NUM_LEDS);
  set_max_power_in_volts_and_milliamps(VOLTS, MAX_CURRENT_IN_MA);
  setupPropShield();
  delay(100);
  // setupWifi();
}

void loop () {
  FastLED.setBrightness(Global::max_bright);
  Global::firstTimeRunningThroughPattern = true;
  uint endTime = millis() + ROTATION_IN_MS;
  while (millis() < endTime) {
    updateMotion();
    patterns[Global::patternId]();
    Global::firstTimeRunningThroughPattern = false;
    show_at_max_brightness_for_power();
  }
  if (maxPatternId > 1) {
    Global::firstTimeRunningThroughPattern = true;
    if (++Global::patternId == maxPatternId) { Global::patternId = 0; }
  }
}
