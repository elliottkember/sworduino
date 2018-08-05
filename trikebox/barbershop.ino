namespace Barbershop {
  int     discoBarberPhase = 0;
  uint8_t discoBarberFrequency = 2;
  uint8_t discoBarberCutoff = 30;
  uint8_t discoBarberSaturation = 240;

  int hue = 0;
  
  void draw() {
    EVERY_N_MILLISECONDS(10) {
      discoBarberPhase += 1;
    }
    
    hue+=1;
      
    for (int k = 0; k < N_LEDS - 1; k++) {
      int _brightness = qsubd(cubicwave8((k * discoBarberFrequency) + discoBarberPhase), discoBarberCutoff);
      Soulmate::led_arr[k] = CHSV(hue + k / 2, discoBarberSaturation, _brightness);
    }
  }
}

void barbershop() {
  Barbershop::draw();
}

