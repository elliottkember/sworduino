uint8_t discoBarberFrequency = 3;
int     discoBarberPhase = 0;
uint8_t discoBarberCutoff = 120;
uint8_t discoBarberSaturation = 240;

// This pattern is called for both Disco Barber and Disco Barber 2.
void discoBarbers() {
  if (patternId == DISCO_BARBER_1) {
    discoBarberFrequency = 5;
    discoBarberPhase += 24;
  } else {
    discoBarberFrequency = 3;
    discoBarberPhase += 48;
  }

  hue = hue + 1;

  for (int k = 0; k < NUM_LEDS - 1; k++) {
    // qsub sets a minimum value called discoBarberCutoff. If < discoBarberCutoff, then bright = 0. Otherwise, bright = 128 (as defined in qsub)..
    int _brightness = qsubd(cubicwave8((k * discoBarberFrequency) + discoBarberPhase), discoBarberCutoff);
    leds[k] = CHSV(0, 0, 0);                                        // First set a background colour, but fully saturated.

    // Sparkles!
    if (random(1000) > 996) {
      leds[k] = CHSV(255, 0, 255);
    }

    if (patternId == DISCO_BARBER_1) {
      leds[k] += CHSV(hue * 10 + k / 2, discoBarberSaturation, _brightness);
    } else {
      leds[k] += CHSV(hue * -20 + k / 4, discoBarberSaturation, _brightness);
    }
  }
}


