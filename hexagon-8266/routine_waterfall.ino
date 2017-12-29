namespace Waterfall {
  uint8_t hue = random(0,255);                 // Starting hue.
  uint8_t numCascades = 1;
  bool increasing = true;
  bool firstTimeRunning = true;
  uint8_t brightness [40];
  bool waterfall() {
    if (firstTimeRunning == true) {
      for (int i=0; i<N_LEDS; i++) {
        Global::led_arr[i] = CHSV(0,0,0);
      }
      firstTimeRunning = false;
    }

    EVERY_N_MILLISECONDS(200) {
      //left
      hue += random(0,42);
      Global::led_arr[37] = CHSV(hue, 255, 255);
      brightness[37] = 255;
      //right
      Global::led_arr[38] = CHSV(hue, 255, 255);
      brightness[38]=255;
    }
    for (int i=18; i<38; i++) {
      if(brightness[i] == 255) {
        Global::led_arr[i-1] = Global::led_arr[i];
        brightness[i-1]=brightness[i];
        
        Global::led_arr[i] = Global::led_arr[i-1];
        Global::led_arr[i].fadeToBlackBy(25);
        brightness[i]=brightness[i]-1;
      } else {
        Global::led_arr[i].fadeToBlackBy(25);
        brightness[i]=brightness[i]-1;
      }
      
    }
   for (int i=17; i>-3; i--) {
    int j=i;
    if (j<0) {
      j += 40;
    }

    if (j==0) {
      Global::led_arr[j] = Global::led_arr[39];
      if (brightness[39] == 255) {
        brightness[0] = 255;
      }
      Global::led_arr[39].fadeToBlackBy(25);
      brightness[39] -= 1;
    } else {
      Global::led_arr[j] = Global::led_arr[j-1];
      if (brightness[j-1] == 255) {
        brightness[j] = 255;
      }
      Global::led_arr[j-1].fadeToBlackBy(25);
      brightness[j-1] -= 1;
    }
   }
    delay(50);
    
    // Terminate randomly
    if(Util::rand_range(0,30)==0){
      return true; // Finished with our game
    }
    return false; // Game ongoing
  }
}

bool waterfall() {
  return Waterfall::waterfall();
}
