// Dave's pattern. By Dave Grijalva with love.
// A gradient background, with rotating bright points and white sparkles.
uint8_t daveOffset = 0;
int daveMilliseconds = 0;
void dave() {
  for (int k=0; k<NUM_LEDS-1; k++) {
    uint8_t k8 = k;
    uint8_t hue = (k/15 * 255) + (daveOffset*5);
    uint8_t saturation = rand() % 255 > 252 ? 0 : 255; //NUM_LEDS - (k+daveOffset*10);
    uint8_t value = k8 % 14 == daveOffset % 14 ? 200 : 40; //NUM_LEDS- (k+daveOffset*10);
    if(saturation == 0){
      value = 255;
    }
    leds[k] = CHSV(hue, saturation, value);
  }
  EVERY_N_MILLISECONDS(1000/24){
    daveMilliseconds++;
    if(daveMilliseconds % 3 == 0){
      daveOffset++;
    }
  }
}
