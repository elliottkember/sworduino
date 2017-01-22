int rotationInMillseconds = 50000;

void (*patterns[])() = {
  beautifulSparkles,
  discoBarber1,
  discoBarber2,
  discoBarber3,
  rain,
  sparkles,
  dave,
  shootingGradients,
  discoTwirl,
  discoTwirl2,
};

int patternId = 0;
int maxPatternId = sizeof( patterns ) / sizeof(patterns[0]);
void loop () {
  Global::firstTimeRunningThroughPattern = true;
  uint endTime = millis() + rotationInMillseconds;
  while (millis() < endTime) {
    patterns[patternId]();
    Global::firstTimeRunningThroughPattern = false;
    show_at_max_brightness_for_power();
  }
  if (maxPatternId > 1) {
    Global::firstTimeRunningThroughPattern = true;
    if (++patternId == maxPatternId) { patternId = 0; }
  }
}
