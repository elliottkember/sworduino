uint8_t hue = 0;

void hexagon() {
  fill_rainbow(Global::led_arr, N_LEDS, hue, 5);
  EVERY_N_MILLISECONDS(10) {
    hue += 1;
  }
}

void fastHexagon() {
  fill_rainbow(Global::led_arr, N_LEDS, hue, 5);
  EVERY_N_MILLISECONDS(2) {
    hue += 5;
  }
}

void slowHexagon() {
  fill_rainbow(Global::led_arr, N_LEDS, hue, 2);
  EVERY_N_MILLISECONDS(40) {
    hue += 1;
  }
}

namespace Rainbow {
  bool initialized = false;
  uint8_t iteration = 0;
  uint8_t increment_every = 1;
  uint8_t hue[N_CELLS];

  void setup(int beam_thickness, uint8_t incr) {
    increment_every = incr;
    int baseline = random(0, 254);
    for(int i=0; i<N_CELLS; i++){
      hue[i] = (i/beam_thickness) + baseline;
    }
  }

  bool iterate() {
    if((iteration % increment_every) == 0){
      for(int i=0; i<N_CELLS; i++){
        hue[i] += 1;
      }
    }
    iteration++;

    return true;
  }

  void paint() {
    for(int i=0; i<N_CELLS; i++){
      Global::led_arr[i] = CHSV(hue[i], 255, 255);
    }
  }

  void reset() {
    initialized = false;
    iteration = 0;
    increment_every = 1;
  }

  bool rainbow(int beam_thickness, uint8_t incr) {
    if(!initialized) {
      setup(beam_thickness, incr);
      initialized = true;
    }

    bool isDone = iterate();
    paint();

    return isDone;
  }
}

void slowPulse() {
  Rainbow::rainbow(25, 4);
}

void fastPulse() {
  Rainbow::rainbow(50, 8);
}
