namespace Flame {
  int flame_new_state[N_CELLS] = {0};
  int flame_state[N_CELLS] = {0};
  int flame_min_temp = 0;
  int flame_max_temp = 255;
  int flame_iterations = 0;
  uint8_t flame_hue_shift = 0;

  void reset() {
    flame_iterations = 0;
  }

  // Heuristic detection of squares that are neighbors only due to wraparound
  bool IsWraparound(int idx1, int idx2) {
    return (abs(idx1-idx2) > (2 * LEDS_PER_ROW));
  }
  
  void flame_set_temperatures(int min_temp_override, int max_temp_override) {
    int mintemp=0;
    int maxtemp=255;
    
    if(min_temp_override>=0){
      mintemp=min_temp_override;
    }
    if(max_temp_override>=0){
      maxtemp=max_temp_override;
    }
    
    flame_min_temp = Util::rand_range(mintemp, maxtemp);
    flame_max_temp = maxtemp;
  }
  
  void flame_setup(int temp_min, int temp_max) {
    // Ignite the last row
    for (int i=0; i<N_CELLS; i++) {
      struct Grid::Point pt = Grid::GetPoint(i);
      if(!IsBottomRow(pt)){
        continue;
      }
      flame_new_state[i] = Util::rand_range(temp_min,temp_max);
    }
  }
  
  void flame_iterate(int temp_min, int temp_max) {
    for(int i=0; i<N_CELLS; i++){
      flame_state[i] = flame_new_state[i];
    }
    
    for(int i=0; i<N_CELLS; i++){
      struct Grid::Point pt = Grid::GetPoint(i);
  
      // Last row doesn't get reset to 0
      if(IsBottomRow(pt)){
        if(Util::rand_range(0,10)==0){
          // Cells in the last row will occasionally vary randomly in intensity
          flame_new_state[i] = Util::rand_range(temp_min,temp_max);
        }
        continue;
      }
  
      // Ceiling is excluded as well
      if(pt.y==0){
        continue;
      }
  
      flame_new_state[i] = 0;
      
      struct Grid::Neighbors nb = Grid::InfiniteGetNeighbors(pt);
      int n_contributing_neighbors = 0;
      for(int j=0; j<8; j++) {
        struct Grid::Point npt = nb.v[j];
        // Skip neighbors above or adjacent to the current square
        if(npt.y<=pt.y){
          continue;
        }
  
        // Skip neighbors too far away -- probably represent wraparound
        int neighborIndex = InfiniteGetIndex(npt);
        if(IsWraparound(neighborIndex, i)){
          continue;
        }
  
        // Use the neighbor's state
        flame_new_state[i] += flame_state[neighborIndex];
        n_contributing_neighbors++;
      }
      flame_new_state[i] = std::max(flame_new_state[i]/std::max(1,n_contributing_neighbors) - Util::rand_range(10,16), 0);
    }
  }
  
  void flame_paint(CRGBPalette256 palette, bool shift_hues) {
    
    for(int i=0; i<N_CELLS; i++){
      uint8_t heatindex = (flame_new_state[i]);
      Global::led_arr[i] = ColorFromPalette(palette, heatindex); // normal palette access
      
      // Draw sparks, on occasion, in the non-burning cells. The sparks are
      // detached from their heat source and so should be relatively cool.
      if((heatindex==0) && (Util::rand_range(0, 10000)==0)){
        Global::led_arr[i] = ColorFromPalette(palette, Util::rand_range(0, 40));
      }
  
      if(shift_hues) {
        // Shift hue. This is really inefficient, but I don't know how to 
        // shift the hue within the palette.
        CHSV hsv = rgb2hsv_approximate( Global::led_arr[i] );
        hsv.hue += flame_hue_shift;
        Global::led_arr[i] = hsv;
      }
    }
  
    // Shift hue for next batch
    if(shift_hues && (Util::rand_range(0, 100)==0)){
      flame_hue_shift += 1;
    }
  }

  bool flame(bool shift_hues, int min_temp_override, int max_temp_override) {
    
    if(flame_iterations == 0){
      flame_set_temperatures(min_temp_override, max_temp_override);
      flame_setup(flame_min_temp, flame_max_temp);
    }
    
    flame_iterate(flame_min_temp, flame_max_temp);
    flame_paint(flame_palette, shift_hues);
  
    flame_iterations++;
  
    // Exit sometimes
    // TODO: Pick nonmagic numbers
    if( (flame_iterations % 1000) == 0){
      flame_set_temperatures(min_temp_override, max_temp_override);
      return true;
    }
    return false;
  }
}

bool flame(bool shift_hues, int min_temp_override, int max_temp_override) {
  return Flame::flame(shift_hues, min_temp_override, max_temp_override);
}
