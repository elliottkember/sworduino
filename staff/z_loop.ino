void (*patterns[])() = {
//  beautifulSparkles,
  rain,
//  sparkles,
//  discoBarber1,
//  dave,
//  discoBarber2,
//  shootingGradients,
//  discoTwirl,
//  discoBarber3,
//  discoTwirl2,
};

int patternId = 0;
int maxPatternId = sizeof( patterns ) / sizeof(patterns[0]);
uint endTime = millis() + ROTATION_IN_MS;
void loop () {
  FastLED.setBrightness(Global::max_bright);
//  Global::firstTimeRunningThroughPattern = true;
//  uint endTime = millis() + ROTATION_IN_MS;
//  while (millis() < endTime) {

  Serial.println(digitalRead(SWITCH_PIN));
  if (digitalRead(SWITCH_PIN) == HIGH) {
    ArduinoOTA.handle();
  }
  patterns[patternId]();
  
  show_at_max_brightness_for_power();
  Global::firstTimeRunningThroughPattern = false;
  delay(1000/60);
  ESP.wdtFeed();

  if (millis() > endTime) {
    Global::firstTimeRunningThroughPattern = false;
    endTime = millis() + ROTATION_IN_MS;
    
    if (maxPatternId > 1) {
      Global::firstTimeRunningThroughPattern = true;
      if (++patternId == maxPatternId) { patternId = 0; }
    }
  }

}
