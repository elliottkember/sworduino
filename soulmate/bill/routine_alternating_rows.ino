bool alternating_rows_randomly() {
  // We'll keep a different row black each iteration
  int alternate_rows = Util::rand_range(0,1);

  if (DEBUG) {
    Serial.println(alternate_rows);
  }

  
  for (int i=0; i<N_LEDS; i++) {
    int row = i / LEDS_PER_ROW;
    
    if (row % 2 == alternate_rows) {
      Soulmate::led_arr[i] = CRGB::Black;
    } else {
      Soulmate::led_arr[i] = CHSV(Util::rand_range(0, 255), 255, 255);
    }
  }

  // Terminate randomly
  if(Util::rand_range(0,30)==0){
    return true; // Finished with our game
  }
  return false; // Game ongoing
}

