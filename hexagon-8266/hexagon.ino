uint8_t hue = 0;
float deltaHue = 5;

void hexagon() {
  fill_rainbow(Global::led_arr, N_LEDS, hue, deltaHue);
  hue += 5;
}
