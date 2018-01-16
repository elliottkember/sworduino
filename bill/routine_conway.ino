namespace Conway {
  CRGB background_color = CHSV( Util::rand_range(0,255), 255, 50);
  bool new_state[N_CELLS] = {false};
  bool state[N_CELLS] = {false};
  bool prev_state[N_CELLS] = {false};

  // Randomly give life to some cells. May help disrupt gliders.
  void add_noise(){
    for (int i=0; i<N_CELLS; i++){
      if(0 == Util::rand_range(0,10*N_CELLS)){
        new_state[i] = true;
      }
    }
  }

  void destroy(CRGB color) {
    delay(1000);

    for(int i=0; i<N_CELLS; i++){
        Soulmate::led_arr[i] = background_color;
        Soulmate::led_arr[i].maximizeBrightness();
        Soulmate::led_arr[i] %= 32; // Now reduce it to N/255 the max brightness
        FastLED.show();
      }
  }

  // Game didn't change between turns
  bool game_frozen() {
    for(int i=0; i<N_CELLS; i++){
      if(state[i] != new_state[i]){
        return false;
      }
    }

    return true;
  }

  // Game oscillated back to previous state
  bool game_oscillating() {
    for(int i=0; i<N_CELLS; i++){
      if(prev_state[i] != new_state[i]){
        return false;
      }
    }

    return true;
  }

  bool game_over() {
    for(int i=0; i<N_CELLS; i++){
      if(new_state[i]){
        return false;
      }
    }

    return true;
  }

  void iterate() {
    for(int i=0; i<N_CELLS; i++){
      prev_state[i] = state[i];
      state[i] = new_state[i];
    }

    for(int i=0; i<N_CELLS; i++){
      struct Grid::Point pt = Grid::InfiniteGetPoint(i);
      struct Grid::Neighbors nearby = Grid::InfiniteGetNeighbors(pt);

      // Count number of living neighbors
      int n_alive = 0;
      for(int i=0; i<8; i++){
        if(state[InfiniteGetIndex(nearby.v[i])]){
          n_alive++;
        }
      }

      if(state[i]){
        // Starts alive

        if(n_alive<2){
          new_state[i] = false;
        } else if(n_alive>=2 && n_alive<=3){
          new_state[i] = true;
        } else if(n_alive>3){
          new_state[i] = false;
        }
      }else{
        // Starts dead

        if(n_alive==3){
          new_state[i] = true;
        }
      }
    }

    add_noise();
  }

  void reset() {
    // Reset the game state:
    for(int i=0; i<N_CELLS; i++){
      new_state[i] = false;
      state[i] = false;
      prev_state[i] = false;
    }

    // Choose a new background color
    background_color = CHSV(Util::rand_range(0, 240), 255, 50);

    // Setup the game state
    for(int i=0; i < N_CELLS; i++){
      if(Util::rand_range(0,10)==0){
        new_state[i] = true;
      }
    }

  //  // Setup the game:
  //  int starting_point = Util::rand_range(0, N_CELLS-1);
  //  struct Point pt = InfiniteGetPoint(starting_point);
  //  struct neighbors nearby = InfiniteGetNeighbors(pt);
  //
  //  for(int i=0; i<8; i++){
  //    if(Util::rand_range(0,1)==1){
  //      new_state[InfiniteGetIndex(nearby.v[i])] = true;
  //    }
  //  }
  }

  void paint() {
    CRGB bgcol = background_color;
    bgcol.maximizeBrightness();

    CRGB oncol = -bgcol;

    for(int i=0; i<N_CELLS; i++){
      if(new_state[i] && !state[i]){
        // Flash into existence
        //led_arr[i] = blend(oncol, bgcol, 200);
        Soulmate::led_arr[i] = oncol;
        Soulmate::led_arr[i].maximizeBrightness();
      }else if(new_state[i]){
        // Stably existing
        Soulmate::led_arr[i] = oncol;
      } else if(state[i]) {
        // Was alive, now dead
        Soulmate::led_arr[i] = oncol;
        Soulmate::led_arr[i] %= 64; // Dim to 25% of its previous value
        Soulmate::led_arr[i] += CRGB( 128, 0, 0); // Make much more red
      }else{
        // Wasn't alive; still isn't
        Soulmate::led_arr[i] = background_color;
      }
    }
  }

  bool conway() {

    EVERY_N_MILLISECONDS(100) {
      iterate();
      paint();
    }

    // Check for termination conditions
    if(game_over()) {
      reset();
      return true; // Finished a game
  //  } else if (game_frozen()) {
  //    for(int i=0; i<N_CELLS; i++){
  //      led_arr[i] = CRGB::Blue;
  //    }
  //    delay(2000);
  //    FastLED.show();
  //    delay(1000);
  //
  //    reset();
    } else if (game_oscillating()) {
      destroy(CRGB::Red);

      reset();

      return true; // Finished a game
    }

    return false; // Game ongoing
  }
}

bool conway() {
  return Conway::conway();
}
