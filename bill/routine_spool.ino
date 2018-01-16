namespace Spool {
  int followers = Util::rand_range(1, N_CELLS/2);
  int leader_position=0;
  uint8_t hue_offset = 0;
  int orientation = true;
  
  bool spool(uint8_t hue_increment) {
    EVERY_N_MILLISECONDS( DELAY ) {
      hue_offset += hue_increment;
    }
    for(int idx=0; idx<N_CELLS; idx++){
      int i = idx;
      if(!orientation){
        i = (N_CELLS-1)-idx;
      }
      
      Soulmate::led_arr[idx] = CRGB(0, 0, 0);
      if((i<=leader_position) && (i>=(leader_position-followers))){
        Soulmate::led_arr[idx] = CHSV(0 + hue_offset, 255, 255);
      }
    }
  
    leader_position++;
    
    if((leader_position-followers)==N_CELLS){
      leader_position = 0;
      orientation = !orientation;
      followers = Util::rand_range(1, N_CELLS/2);
      return true;
    }
  
    return false;
  }

  void reset() {
    hue_offset = 0;
    leader_position = 0;
    orientation = true;
  }
}

bool spool() {
  return Spool::spool(0);
}

bool rainbow_spool() {
  return Spool::spool(1);
}

