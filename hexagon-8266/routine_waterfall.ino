namespace Waterfall {
  uint8_t hue = random(0,255);   
  uint8_t hueRight = random(0,255);  // Starting hue.
  uint8_t numCascades = 1;
  uint8_t fadeBy = 0;
  bool increasing = true;
  bool firstTimeRunning = true;
  uint8_t brightness [40];
  bool waterfall() {

    EVERY_N_MILLISECONDS(300) {
      //left
      uint8_t hueOffset = random(0,3);
      if (hueOffset == 0){
        hue += 21 * random(-1,1);
      } else if (hueOffset == 1) {
        if (random(1,100) <50 ) {
          hue += 127;
        } else {
          hue += 5 * random(-1,1);
        }
      } else if (hueOffset == 2) {
        if (random(1,100) <80 ) {
          hue += 85 * random(-1,1);
        } else {
          hue += 5 * random(-1,1);
        }
      } else if (hueOffset == 3){
        hue += 64 * random(-1,1);
      }
      hueRight=hue;
      int hueRand = random(0,10);
      if (hueRand > 8) {
        if (hueRand == 9) {
          hueRight += 85 * random(-1,1);
        } else if (hueRand == 10){
          hueRight += 64 * random(-1,1);
        }
      }
      
      Global::led_arr[37] = CHSV(hue, random(180,210), 255);
      brightness[37] = 255;
      //right
      Global::led_arr[38] = CHSV(hueRight, random(180,210), 255);
      brightness[38]=255;
    }
    for (int i=18; i<38; i++) {
      if(brightness[i] == 255) {
        Global::led_arr[i-1] = Global::led_arr[i];
        brightness[i-1]=brightness[i];
        
        Global::led_arr[i] = Global::led_arr[i-1];
        Global::led_arr[i].fadeToBlackBy(fadeBy);
        brightness[i]=brightness[i]-1;
      } else {
        Global::led_arr[i].fadeToBlackBy(fadeBy);
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
      Global::led_arr[39].fadeToBlackBy(fadeBy);
      brightness[39] -= 1;
    } else {
      if (j ==38) {
        Global::led_arr[j-1].fadeToBlackBy(fadeBy);
        brightness[j-1] -= 1;
      } else {
        Global::led_arr[j] = Global::led_arr[j-1];
        if (brightness[j-1] == 255) {
          brightness[j] = 255;
        }
        Global::led_arr[j-1].fadeToBlackBy(fadeBy);
        brightness[j-1] -= 1;
      }
    }
   }
    delay(30);
    
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
