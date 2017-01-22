void loop () {
  if (!holdPattern) {
    EVERY_N_MILLISECONDS(rotationInMillseconds) {
      firstTimeRunningThroughPattern = true;
      patternId += 1;
      if (patternId > maxPatternId) {
        patternId = 1;
      }
    }
  }

  if (patternId == NIGHT_SPARKLES) {
    nightSparkles();
  } else if (patternId == BEAUTIFUL_SPARKLES) {
    beautifulSparkles();
  } else if (patternId == SINGLE_COLOR_SPARKLES) {
    sparkles();
  } else if (patternId == DISCO_BARBER_1 || patternId == DISCO_BARBER_2) {
    discoBarbers();
  } else if (patternId == SHOOTING_GRADIENTS) {
    shootingGradients();
  } else if (patternId == DISCO_TWIRL) {
    discoTwirl();
  } else if (patternId == DISCO_TWIRL_2) {
    discoTwirl2();
  } else if (patternId == DAVE) {
    dave();
  }

  firstTimeRunningThroughPattern = false;

  show_at_max_brightness_for_power();
}
