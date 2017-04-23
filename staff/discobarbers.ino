namespace DiscoBarbers {
  int frequency;
  int phase;
  // int hue = 0;
  uint8_t cutoff = 120;
  uint8_t saturation = 240;

  void discoBarber1() {
    frequency = 5;
    phase += 20;
    
    // Special CPP11 closure syntax
    Global::printPixels([](int k) -> CRGB {
      int _brightness = qsubd(cubicwave8((k * frequency) + phase), cutoff);
      if (_brightness == 0) {
        if (random16(1000) > 995) { return CHSV(255, 0, 255); }
      }
      // if (random16(1000) > 996) { return CHSV(255, 0, 64); }
      return CHSV(Global::hue * 10 + k - phase, saturation, _brightness);
    });
  }

  void discoBarber2() {
    phase -= 8;
    // cutoff = 120;
    saturation = 240;
    frequency = 3;
    Global::printPixels([](int k) -> CRGB {
      if (random16(1000) > 996) { return CHSV(255, 0, 255); }
      int _brightness = qsubd(cubicwave8((k * frequency) + phase), cutoff);
      return CHSV(Global::hue * -20 + k / 4 + phase, saturation, _brightness);
    });
  }

//  namespace db3 {
//    uint8_t counter = 0;
//    int interference = 0;
//    bool zoom = false;
//    int zoomCounter = 0;
//
//    void discoBarber3() {
//      counter += 1;
//      if (zoom) zoomCounter++;
//      int beat = beatsin8(110, 0, 200);
//
//      EVERY_N_MILLISECONDS(1000) {
//        if (zoom == false && beat < 10) {
//          zoom = true;
//          zoomCounter = 0;
//        }
//      }
//
//      if (!zoom) {
//        interference = 0;
//      } else {
//        interference = beat;
//        if (interference < 10 && zoomCounter > 10) {
//          zoom = false;
//        }
//      }
//      
//      Global::printPixels([](int k) -> CRGB {
//        int _h = counter + ((double)k / (double)NUM_LEDS) * 30.0; // quadwave8(counter); // Global::hue * 10 + k;
//        int _s = 255 - quadwave8(255 / k);
//        int _v = _max(0, quadwave8(counter * 10 + k / 2) - interference);
//        return CHSV(_h, _s, _v);
//      });
//    }
//  }
//
//  void discoBarber3() {
//    db3::discoBarber3();
//  }

  void discoBarbers(int (*calculateHue)(int, int)) {
    Global::hue = Global::hue + 1;
    for (int k = 0; k < NUM_LEDS - 1; k++) {
      // qsub sets a minimum value called cutoff.
      // If < cutoff, then bright = 0. Otherwise, bright = 128 (as defined in qsub)..
      int _brightness = qsubd(cubicwave8((k * frequency) + phase), cutoff);
      Global::leds[k] = CHSV(0, 0, 0);                                        // First set a background colour, but fully saturated.
      // Sparkles!
      if (random16(1000) > 996) { Global::leds[k] = CHSV(255, 0, 255); }
      Global::leds[k] += CHSV(calculateHue(k, Global::hue) / 4, saturation, _brightness);
    }
  }
}


void discoBarber1() {
  DiscoBarbers::discoBarber1();
}
void discoBarber2() {
  DiscoBarbers::discoBarber2();
}
//void discoBarber3() {
//  DiscoBarbers::discoBarber3();
//}
