namespace NewPattern {
  int SPARKLE_COUNT = 50;
  uint16_t sparkles[50];

  CRGBPalette16 currentPalette(CRGB::Black);
  CRGBPalette16 targetPalette(PartyColors_p );
  TBlendType currentBlending; // NOBLEND or LINEARBLEND

  void FillLEDsFromPaletteColors(uint8_t colorIndex) {
    for(int i = 0; i < NUM_LEDS; i++) {
      Global::leds[i] = ColorFromPalette(currentPalette, colorIndex + sin8(i*16), 255);
      colorIndex += 3;
    }
  }

  void ChangePalettePeriodically() {
    uint8_t secondHand = (millis() / 1000) % 60;
    static uint8_t lastSecond = 99;

    if(lastSecond != secondHand) {
      lastSecond = secondHand;
      CRGB p = CHSV(HUE_PURPLE, 255, 255);
      CRGB g = CHSV(HUE_GREEN, 255, 255);
      CRGB b = CRGB::Black;
      CRGB w = CRGB::White;
      if(secondHand ==  0)  { targetPalette = RainbowColors_p; }
      if(secondHand == 10)  { targetPalette = CRGBPalette16(g,g,b,b, p,p,b,b, g,g,b,b, p,p,b,b); }
      if(secondHand == 20)  { targetPalette = CRGBPalette16(b,b,b,w, b,b,b,w, b,b,b,w, b,b,b,w); }
      if(secondHand == 30)  { targetPalette = LavaColors_p; }
      if(secondHand == 40)  { targetPalette = CloudColors_p; }
      if(secondHand == 50)  { targetPalette = PartyColors_p; }
    }
  }

  void loop() {
    ChangePalettePeriodically();
    // nblendPaletteTowardPalette() will crossfade current palette slowly toward the target palette.
    // Each time that nblendPaletteTowardPalette is called, small changes
    // are made to currentPalette to bring it closer to matching targetPalette.
    // You can control how many changes are made in each call:
    //   - the default of 24 is a good balance
    //   - meaningful values are 1-48.  1=veeeeeeeery slow, 48=quickest
    //   - "0" means do not change the currentPalette at all; freeze
    EVERY_N_MILLISECONDS(100) {
      uint8_t maxChanges = 24;
      nblendPaletteTowardPalette(currentPalette, targetPalette, maxChanges);
    }

    static uint8_t startIndex = 0;
    startIndex += 1;
    FillLEDsFromPaletteColors(startIndex);
  }
}

void newPattern() {
  NewPattern::loop();
}
