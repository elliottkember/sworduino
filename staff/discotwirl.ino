int thisphase = 0;                // Phase change value gets calculated.
bool fadeUp = 0;
bool thisdir = 0;                 // You can change direction.
int8_t thisspeed = 16;            // You can change the speed, and use negative values.
uint8_t thishue = 0;              // You can change the starting hue value for the first wave.
uint8_t thisrot = 18;             // You can change how quickly the hue rotates for this wave. Currently 0.
uint8_t allsat = 255;             // I like 'em fully saturated with colour.
uint8_t allfreq = 1;              // You can change the frequency, thus overall width of bars.
uint8_t thiscutoff = 200;         // You can change the cutoff value to display this wave. Lower value = longer wave.
uint8_t fade = 200;

void discoTwirl() {

  if (Global::firstTimeRunningThroughPattern) {
    thishue = 0;           // You can change the starting hue value for the first wave.
    thisrot = 18;          // You can change how quickly the hue rotates for this wave. Currently 0.
    allsat = 180;          // I like 'em fully saturated with colour.
    thisdir = 0;           // You can change direction.
    thisspeed = 8;         // You can change the speed, and use negative values.
    allfreq = 4;           // You can change the frequency, thus overall width of bars.
    thisphase = 0;         // Phase change value gets calculated.
    thiscutoff = 200;      // You can change the cutoff value to display this wave. Lower value = longer wave.
    fade = 200;
    fadeUp = 1;
    fadeToBlackBy(Global::leds, NUM_LEDS, 255);
  } else {
    EVERY_N_MILLISECONDS(2) {
      if (fadeUp) fade += 10; else fade -= 10;
      if (fade < 50) fadeUp = 1; else if (fade > 250) fadeUp = 0;
    }
  }

  if (thisdir == 0) thisphase+=thisspeed; else thisphase-=thisspeed;          // You can change direction and speed individually.
  fadeToBlackBy(Global::leds, NUM_LEDS, fade);
  for (int k=0; k<NUM_LEDS-1; k++) {
    int thisbright = cubicwave8((k*-allfreq)+thisphase);      // qsub sets a minimum value called thiscutoff. If < thiscutoff, then bright = 0. Otherwise, bright = 128 (as defined in qsub)..
    Global::leds[k] += CHSV(thishue + k + thisphase / 5, 200, thisbright);                               // Assigning hues and brightness to the led array.
  }
}

void discoTwirl2() {

  if (Global::firstTimeRunningThroughPattern) {
    thishue = 0;                                          // You can change the starting hue value for the first wave.
    thisrot = 18;                                          // You can change how quickly the hue rotates for this wave. Currently 0.
    allsat = 255;                                         // I like 'em fully saturated with colour.u¨
    thisdir = 0;                                             // You can change direction.
    thisspeed = 16;                                         // You can change the speed, and use negative values.
    allfreq = 1;                                         // You can change the frequency, thus overall width of bars.
    thisphase = 0;                                            // Phase change value gets calculated.
    thiscutoff = 200;                                     // You can change the cutoff value to display this wave. Lower value = longer wave.
    fade = 200;
    fadeUp = 0;
    thisrot = 0;                                          // You can change how quickly the hue rotates for this wave. Currently 0.
    allsat = 255;                                         // I like 'em fully saturated with colour.
    thisdir = 0;                                             // You can change direction.
    thisspeed = 32;                                         // You can change the speed, and use negative values.
    allfreq = 32;                                         // You can change the frequency, thus overall width of bars.
    thisphase = 0;                                            // Phase change value gets calculated.
    thiscutoff = 200;                                     // You can change the cutoff value to display this wave. Lower value = longer wave.
  }
  if (thisdir == 0) thisphase+=thisspeed; else thisphase-=thisspeed;          // You can change direction and speed individually.
  fadeToBlackBy(Global::leds, NUM_LEDS, fade);
  for (int k=0; k<NUM_LEDS-1; k++) {
    int thisbright = qsubd(cubicwave8((k*-allfreq)+thisphase), thiscutoff);      // qsub sets a minimum value called thiscutoff. If < thiscutoff, then bright = 0. Otherwise, bright = 128 (as defined in qsub)..
    Global::leds[k] += CHSV(thishue + k + thisphase / 5, allsat, thisbright);                               // Assigning hues and brightness to the led array.  }
  }
}
