#define N_Spots 6

namespace Puff {
  struct Spot {
    struct Grid::Point Center;
    uint32_t Age;
    uint8_t Radius;
    CHSV hsv;
    bool valid;
    uint8_t MaxBrightness; // from 0 (black) to 255 (max)
  };

  uint32_t MaxAge = 20; // Iterations on-screen (multiply by "lifetime" for max allowed duration [in ms] on screen)
  uint8_t MaxRadius = 3; // Pixels
  uint16_t lifetime = DELAY; // milliseconds
  unsigned long last = millis();
  bool initialized = false;
  struct Spot state[N_Spots];
  
  struct Cloud {
    CHSV v[N_CELLS] = {};
  };
  
  struct Cloud circle(struct Spot spot) {
    int radius = spot.Radius;
    int x = -radius;
    int y = 0;
    int err = 2 - 2 * radius;
  
    struct Cloud cloud;
    for (int i = 0; i < N_CELLS; i++) {
      cloud.v[i] = CHSV(0, 0, 0);
    }

    if( DEBUG ) {
      Serial.printf("Radius: %d\n", radius);
    }
  
    while (x < 0) {
      cloud.v[Grid::InfiniteGetIndex(Grid::Point{spot.Center.x - x, spot.Center.y + y})] = spot.hsv;
      cloud.v[Grid::InfiniteGetIndex(Grid::Point{spot.Center.x - y, spot.Center.y - x})] = spot.hsv;
      cloud.v[Grid::InfiniteGetIndex(Grid::Point{spot.Center.x + x, spot.Center.y - y})] = spot.hsv;
      cloud.v[Grid::InfiniteGetIndex(Grid::Point{spot.Center.x + y, spot.Center.y + x})] = spot.hsv;
      radius = err;
      if (radius <= y) {
        err += ++y * 2 + 1;
      }
      if (radius > x || err > y) {
        err += ++x * 2 + 1;
      }
    }
  
    return cloud;
  }
  
  struct Spot randomSpot() {
    struct Spot s;
    
    s.Age = 0;
    s.Radius = Util::rand_range(0, 1);
    s.Center = Grid::InfiniteGetPoint(Util::rand_range(0, N_CELLS - 1));
    s.hsv = CHSV(Util::rand_range(0, 255), 255, 255);
    s.valid = true;
    s.MaxBrightness = 0;
  
    return s;
  }

  double Nearest(struct Spot s) {
    double minDist = -1;
    for(int i=0; i<N_Spots; i++){
      double dist = sqrt(pow(state[i].Center.y - s.Center.y, 2) + pow(state[i].Center.x - s.Center.x, 2));
      if((minDist == -1) || (dist < minDist)) {
        minDist = dist;
      }
    }

    return minDist;
  }
  
  void setup() {
    for (int i = 0; i < N_Spots; i++) {
      state[i].Center = Grid::Point{0, 0};
      state[i].Radius = 0;
      state[i].Age = 0;
      state[i].MaxBrightness = 0;
      state[i].hsv = CHSV(0, 0, 0);
      state[i].valid = false;
    }
  
    // Set valid points
    for(int i=0; i<N_Spots; i++){
      state[i] = randomSpot();
      state[i].Age = Util::rand_range(0, MaxAge);
    }
  
    initialized = true;
  }
  
  void reset() {
    initialized = false;
  }
  
  void iterate() {
    if ( (millis() - last) < lifetime) {
      return;
    }
    last = millis();
  
    for (int i = 0; i < N_Spots; i++) {
      state[i].Age += 1;

      // Young spots are brightened until they achieve max brightness
      if(state[i].Age < MaxAge) {
        // Shift the center up 1 row to generate a rising effect
        if(state[i].MaxBrightness < 80) {
          state[i].Center.y -= 1;
          state[i].Center.x -= 1;
          state[i].Radius = 1;
        } else {
          if(state[i].Radius < MaxRadius) {
            state[i].Radius = (state[i].Radius + 1);
          }
        }
        
        if(state[i].MaxBrightness < 235) {
          state[i].MaxBrightness += 20;
        }else{
          state[i].MaxBrightness = 255;
        }
      }

      // Old spots get modified and eventually killed
      if(state[i].Age > MaxAge) {

        // Shift the center down N rows to generate a falling effect, 
        // if that is desired.
        if(state[i].MaxBrightness < 120) {
          state[i].Center.y += 0;
        }
        
        // Prevent Radius overflow
        state[i].Radius = MaxRadius;

        // Prevent brightness underflow
        if(state[i].MaxBrightness >  40) {
          state[i].MaxBrightness -= 30;
        } else {
          state[i].MaxBrightness = 30;
        }
      }
  
      // Probabilistically replace Aged-out spots once they are blackened
      if((state[i].Age >= MaxAge) && (state[i].MaxBrightness == 30)) {
        // With 1/N probability, generate a new spot
        if(Util::rand_range(0,1) != 0) {
          continue;
        }
        
        // Try not to let the new spot be too close to another one
        // Recenter it if it is
        struct Spot s = randomSpot();
        for(int j = 0; j < N_Spots * 3; j++){
          if(Nearest(s) < 4) {
            s.Center = Grid::InfiniteGetPoint(Util::rand_range(0, N_CELLS - 1));
            continue;
          }
          break;
        }

        state[i].Radius = s.Radius;
        state[i].Age = 0;
        state[i].Center = s.Center;
        state[i].hsv = s.hsv;
        state[i].valid = s.valid;
        state[i].MaxBrightness = s.MaxBrightness;
      }
    }
  }
  
  void paint() {
    CHSV cells[N_CELLS] = {};
    for (int i = 0; i < N_CELLS; i++) {
      cells[i] = CHSV(0, 0, 0);
    }
  
    for (int i = 0; i < N_Spots; i++) {
      if (!state[i].valid) {
        continue;
      }
      struct Spot spot = state[i];
  
      // For each valid spot, make concentric circles based on its radius
      for (int k = 0; k <= spot.Radius; k++) {      
        struct Spot partial = spot;
        partial.Radius -= k;
        partial.hsv.v = map((spot.MaxBrightness / MaxRadius) * (1+k), 0, ((1+MaxRadius)*255/MaxRadius), 0,255);
        
        // Assign color
        struct Cloud cloud = circle(partial);
        for (int j = 0; j < N_CELLS; j++) {
          if (cloud.v[j] == CHSV(0, 0, 0)) {
            continue;
          }

          // Blend if cell is already colored
          if(cells[j] != CHSV(0, 0, 0)) {
            cells[j] = blend(cells[j], cloud.v[j], 128);
          } else {
            cells[j] = cloud.v[j];
          }
        }
      }

      if( DEBUG ) {
        Serial.printf("Cell: %d, Radius: %d\n", i, spot.Radius);
      }
    }

    // Set the global LED array
    for (int i = 0; i < N_CELLS; i++) {
      Soulmate::led_arr[i] = cells[i];
    }
  }
  
  bool puff() {
    if (!initialized) {
      setup();
    }
  
    iterate();
    paint();
  
    return false;
  }
}

bool puff() {
  return Puff::puff();
}

