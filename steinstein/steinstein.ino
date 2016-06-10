#include "FastLED.h";
#define LED_DT 7              // Data pin to connect to the strip.
#define COLOR_ORDER GRB       // Are they RGB, GRB or what??
#define LED_TYPE WS2812B      // Don't forget to change LEDS.addLeds
#define DOTS_PER_LOOP 8
#define DISTANCE_BETWEEN_DOTS 200
#define DISTANCE_BETWEEN_LOOPS 200
#define MAX_EFFECTIVE_DISTANCE_SQUARED 300 * 300
#define NUM_LEDS 1500
#define MAX_DELTA 16
#define MAX_BRIGHTNESS 48
#define MIN_BRIGHTNESS 2
#define MAX_DROPS 6
struct CRGB leds[NUM_LEDS];   // Initialize our LED array.
struct CRGB stein_next[NUM_LEDS]; 


void setup() {
  delay(1000);
  Serial.begin(57600);
  LEDS.addLeds<LED_TYPE, LED_DT, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(255);
  set_max_power_in_volts_and_milliamps(5, 10000);
  randomSeed(analogRead(0));

  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB(0, 0, 0);
  }
}

int counter;
int GAIN_NUM [3] = {10, 10, 10};

void loop() {
  counter++;
  if (counter % 5 == 0){
    int hue_target = random(270);
    for (int rgb_idx = 0; rgb_idx < 3; rgb_idx++)
       GAIN_NUM[rgb_idx] = 8 + ((hue_target + 90 * rgb_idx) % 270) / 58;
//    int gain_seed = random(6);
//    for (int rgb_idx = 0; rgb_idx < 3; rgb_idx++)
//       GAIN_NUM[rgb_idx] = 8 + gain_seed;
  }
  for (int i = 0; i < NUM_LEDS; i++) {
    stein_next[i] = CRGB(leds[i].red, leds[i].g, leds[i].b);
  }

  for (int pixel_k=0; pixel_k<NUM_LEDS; pixel_k++){
    int pixel_ks_next_step [3] = { leds[pixel_k].r, leds[pixel_k].g, leds[pixel_k].b };
    int pixel_ks_last_step [3] = { leds[pixel_k].r, leds[pixel_k].g, leds[pixel_k].b };
    int pixel_ks_weight = 0;
    for (int pixel_m=max(pixel_k-10, 0); pixel_m<min(pixel_k+10, NUM_LEDS); pixel_m++){
      // how far apart are k and m?
      int x_distance = (DISTANCE_BETWEEN_DOTS * (max(pixel_k-pixel_m, pixel_m-pixel_k) % DOTS_PER_LOOP));
      int y_distance = (DISTANCE_BETWEEN_LOOPS * (max(pixel_k-pixel_m, pixel_m-pixel_k) / DOTS_PER_LOOP));
      int distance_squared = x_distance * x_distance + y_distance * y_distance;
      // are they close?
      if (distance_squared < MAX_EFFECTIVE_DISTANCE_SQUARED && pixel_k != pixel_m){
        // this pixel changes my next value
        int pixel_m_color [3] = { leds[pixel_m].r, leds[pixel_m].g, leds[pixel_m].b };
        for (int rgb_idx = 0; rgb_idx < 3; rgb_idx++){
          pixel_ks_next_step[rgb_idx] = GAIN_NUM[rgb_idx] * (pixel_ks_weight * pixel_ks_next_step[rgb_idx] + distance_squared * pixel_m_color[rgb_idx]) / (10 * (pixel_ks_weight + distance_squared));
        }
        pixel_ks_weight += distance_squared;
      }
    }
    for (int rgb_idx = 0; rgb_idx < 3; rgb_idx++){
      if (pixel_ks_last_step[rgb_idx] - pixel_ks_next_step[rgb_idx] > MAX_DELTA){
        stein_next[pixel_k][rgb_idx] = pixel_ks_last_step[rgb_idx] - MAX_DELTA;
      } else if (pixel_ks_last_step[rgb_idx] - pixel_ks_next_step[rgb_idx] < 0-MAX_DELTA) {
        stein_next[pixel_k][rgb_idx] = pixel_ks_last_step[rgb_idx] + MAX_DELTA;
      } else {
        stein_next[pixel_k][rgb_idx] = pixel_ks_next_step[rgb_idx];
      }
      stein_next[pixel_k][rgb_idx] = max(MIN_BRIGHTNESS, min(stein_next[pixel_k][rgb_idx], MAX_BRIGHTNESS));
    }
  }

  for (int k = 0; k<NUM_LEDS; k++){
    leds[k] = stein_next[k];
  }
  for (int i = 0; i<random(MAX_DROPS); i++){
    int k = random(NUM_LEDS);
    if (k > DOTS_PER_LOOP + 1)
      leds[k-DOTS_PER_LOOP-1] = CRGB(random(MAX_BRIGHTNESS), random(MAX_BRIGHTNESS), random(MAX_BRIGHTNESS));
    if (k > DOTS_PER_LOOP)
      leds[k-DOTS_PER_LOOP] = CRGB(random(MAX_BRIGHTNESS), random(MAX_BRIGHTNESS), random(MAX_BRIGHTNESS));
    if (k > 1)
      leds[k-1] = CRGB(random(MAX_BRIGHTNESS), random(MAX_BRIGHTNESS), random(MAX_BRIGHTNESS));
    leds[k] = CRGB(random(MAX_BRIGHTNESS), random(MAX_BRIGHTNESS), random(MAX_BRIGHTNESS));
  }
  
  show_at_max_brightness_for_power();
}

