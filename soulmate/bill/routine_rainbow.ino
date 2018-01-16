namespace Rainbow {
  bool initialized = false;
  uint8_t iteration = 0;
  uint8_t increment_every = 1;
  uint8_t hue[N_CELLS];

  void setup(int beam_thickness, uint8_t incr) {
    increment_every = incr;
    int baseline = Util::rand_range(0,255);
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
      Soulmate::led_arr[i] = CHSV(hue[i], 255, 255);
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

bool pulse() {
  return Rainbow::rainbow(100, 1);
}

bool slowPulse() {
  return Rainbow::rainbow(25, 4);
}

bool slowestPulse() {
  return Rainbow::rainbow(200, 100);
}

bool rainbow() {
  return Rainbow::rainbow(3, 2);
}
