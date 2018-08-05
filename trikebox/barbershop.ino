int     discoBarberPhase = 0;
uint8_t discoBarberFrequency = 10;
uint8_t discoBarberCutoff = 30;
uint8_t discoBarberSaturation = 240;

void barbershop() {
  int hue = 0;
  discoBarberFrequency = 2;

  EVERY_N_MILLISECONDS(10) {
    discoBarberPhase += 1;
  }
  
  EVERY_N_MILLISECONDS(100) {
    hue = hue + 1;
  }

  for (int k = 0; k < N_LEDS - 1; k++) {
    int _brightness = qsubd(cubicwave8((k * discoBarberFrequency) + discoBarberPhase), discoBarberCutoff);
    Soulmate::led_arr[k] = CHSV(hue * 10 + k / 2, discoBarberSaturation, _brightness);                             // Then assign a hue to any that are bright enough.
  }
}
