namespace Solid {
  bool solid(uint8_t hue, uint8_t saturation, uint8_t value) {
    for(int i=0; i<N_LEDS; i++){
      Soulmate::led_arr[i] = CHSV(hue, saturation, value);
    }

    return true;
  }
}

bool black() {
  return Solid::solid(0, 0, 0);
}

bool white() {
  return Solid::solid(24, 192, 255);
}

bool warm_white() {
  return Solid::solid(16, 224, 255);
}
