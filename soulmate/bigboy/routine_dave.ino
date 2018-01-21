namespace Dave {
  uint16_t daveOffset = 0;
  uint8_t daveMilliseconds = 0;
  
  void draw() {
    for (int k=0; k<N_LEDS-1; k++) {
      uint16_t k8 = k;
      uint16_t hue = (k/15 * 255) + (daveOffset*5);
      uint16_t saturation = rand() % 255 > 252 ? 0 : 255; //NUM_LEDS - (k+daveOffset*10);
      uint16_t value = k8 % 14 == daveOffset % 14 ? 200 : 40; //NUM_LEDS- (k+daveOffset*10);
      if(saturation == 0){
        value = 255;
      }
      Soulmate::led_arr[k] = CHSV(hue, saturation, value);
    }
    EVERY_N_MILLISECONDS(DELAY){
      daveMilliseconds++;
      if(daveMilliseconds % 3 == 0){
        daveOffset++;
      }
    }
    delay(60);
  }
}

bool dave() {
  Dave::draw();

  // Terminate randomly
  if(Util::rand_range(0,30)==0){
    return true; // Finished with our game
  }
  return false; // Game ongoing
}
