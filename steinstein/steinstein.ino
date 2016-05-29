#include "FastLED.h";
#define LED_DT 7              // Data pin to connect to the strip.
#define COLOR_ORDER GRB       // Are they RGB, GRB or what??
#define LED_TYPE WS2812B      // Don't forget to change LEDS.addLeds
#define DOTS_PER_LOOP 8
#define DISTANCE_BETWEEN_DOTS 200
#define DISTANCE_BETWEEN_LOOPS 100
#define MAX_EFFECTIVE_DISTANCE_SQUARED 400 * 400
#define NUM_LEDS 1500
struct CRGB leds[NUM_LEDS];   // Initialize our LED array.

struct CRGB last[NUM_LEDS]; 
struct CRGB stein_next[NUM_LEDS]; 


void setup() {
  delay(1000);
  Serial.begin(57600);
  LEDS.addLeds<LED_TYPE, LED_DT, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(255);
  set_max_power_in_volts_and_milliamps(5, 10000);
  randomSeed(analogRead(0));

  for (int i = 0; i < NUM_LEDS; i++) {
    last[i] = CRGB(0, 0, 0);
  }
}

int counter;
  
void loop() {

  counter++;
  int GAIN_NUM [3] = { random(6) + 9, random(6) + 9, random(6) + 9 };

  for (int i = 0; i < NUM_LEDS; i++) {
    stein_next[i] = CRGB(last[i].red, last[i].g, last[i].b);
  }

  for (int pixel_k=0; pixel_k<NUM_LEDS; pixel_k++){
    int pixel_ks_next_step [3] = { last[pixel_k][0], last[pixel_k][1], last[pixel_k][2] };
    int pixel_ks_weight = 0;
    for (int pixel_m=max(pixel_k-10, 0); pixel_m<min(pixel_k+10, NUM_LEDS); pixel_m++){
      // how far apart are k and m?
      int x_distance = (DISTANCE_BETWEEN_DOTS * (abs(pixel_k-pixel_m) % DOTS_PER_LOOP));
      int y_distance = (DISTANCE_BETWEEN_LOOPS * (floor(abs(pixel_k-pixel_m) / DOTS_PER_LOOP)));
      int distance_squared = x_distance * x_distance + y_distance * y_distance;
      // are they close?
      if (distance_squared < MAX_EFFECTIVE_DISTANCE_SQUARED && pixel_k != pixel_m){
        // this pixel changes my next value
        int pixel_m_color [3] = { last[pixel_m][0], last[pixel_m][1], last[pixel_m][2] };
        for (int rgb_idx = 0; rgb_idx < 3; rgb_idx++)
          pixel_ks_next_step[rgb_idx] = GAIN_NUM[rgb_idx] * (pixel_ks_weight * pixel_ks_next_step[rgb_idx] + distance_squared * pixel_m_color[rgb_idx]) / (10 * (pixel_ks_weight + distance_squared));
        pixel_ks_weight += distance_squared;
      }
    }
    for (int rgb_idx = 0; rgb_idx < 3; rgb_idx++)
      stein_next[pixel_k][rgb_idx] = pixel_ks_next_step[rgb_idx];
  }

  for (int k = DOTS_PER_LOOP+1; k<NUM_LEDS; k++){
    last[k] = stein_next[k];
    if (random(10000) > 9995){
      last[k-DOTS_PER_LOOP-1] = CRGB(random(255), random(255), random(255));
      last[k-DOTS_PER_LOOP] = CRGB(random(255), random(255), random(255));
      last[k-1] = CRGB(random(255), random(255), random(255));
      last[k] = CRGB(random(255), random(255), random(255));
    }
    if (random(255) > 230){
      for (int rgb_idx = 0; rgb_idx < 3; rgb_idx++){
        last[k-DOTS_PER_LOOP-1][rgb_idx] = 0;
        last[k-DOTS_PER_LOOP][rgb_idx] = 0;
        last[k-1][rgb_idx] = 0;
        last[k][rgb_idx] = 0;
      }
    }
  }

  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB(min(last[i][0], 64), min(last[i][1], 64), min(last[i][2], 64));
  }
  
    show_at_max_brightness_for_power();
//      set_leds(dither(0, 64, 4, stein_next));
}

