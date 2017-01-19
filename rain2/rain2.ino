/*
simlated a mesh of springs, where brightness is the height each light
is away from the "mesh"
*/

#include "FastLED.h"
#define LED_DT 7              // Data pin to connect to the strip.
#define COLOR_ORDER GRB       // Are they RGB, GRB or what??
#define LED_TYPE WS2812B      // Don't forget to change LEDS.addLeds
#define TRUE_LED_COUNT 300
#define NUM_LEDS 150 //1500

#define L 8 // LIGHTS_PER_WRAP
#define O 2 // LOOP_OFFSET_MM
#define W 10 // HORIZONTAL_SPACE_MM
#define H 10 // VERTICAL_SPACE_MM

#define K 3 // SPRING_CONST

#define VEL_DAMPING .9 // number between 0 and 1;
#define MAX_VEL 1.5
#define FALL_SPEED 100

struct CRGB leds[TRUE_LED_COUNT];
float aleds_vel[NUM_LEDS];
float aleds_brightness[NUM_LEDS];
float anext_leds_brightness[NUM_LEDS];
float bleds_vel[NUM_LEDS];
float bleds_brightness[NUM_LEDS];
float bnext_leds_brightness[NUM_LEDS];

int counter = 0;

///* begin neighborhood
int neighbor_count = 8;
float neighbor_dist[8];
int idx_offset[8];

void init_helper_values() {
  neighbor_dist[0] = sqrt((H*H)+((W-O)*(W-O)));
  neighbor_dist[1] = sqrt((H*H)+(O*O));
  neighbor_dist[2] = sqrt((H*H)+((W+O)*(W+O)));
  neighbor_dist[3] = W;
  neighbor_dist[4] = neighbor_dist[0];
  neighbor_dist[5] = neighbor_dist[1];
  neighbor_dist[6] = neighbor_dist[2];
  neighbor_dist[7] = neighbor_dist[3];

  idx_offset[0] = -L-1;
  idx_offset[1] = -L;
  idx_offset[2] = -L+1;
  idx_offset[3] = -1;
  idx_offset[4] = 1;
  idx_offset[5] = L-1;
  idx_offset[6] = L;
  idx_offset[7] = L+1;
}
//*/ // end neighborhood



void setup() {
  delay(1000);
  Serial.begin(57600);
  LEDS.addLeds<LED_TYPE, LED_DT, COLOR_ORDER>(leds, TRUE_LED_COUNT);
  FastLED.setBrightness(255);
  set_max_power_in_volts_and_milliamps(5, 10000);
  randomSeed(analogRead(0));
  init_helper_values();

  for (int i = 0; i < TRUE_LED_COUNT; i++) {
    leds[i] = CRGB(0, 0, 0);
  }
}

float accel(float brightness, float dist){
  // assume a mass of one and compute the force vector from the sprint.
  float delta = (sqrt((dist*dist)+(brightness*brightness)) - dist);
  float force = K * delta;
  // only want the vertical component of the force vector
  return force * (brightness / (delta + dist));
}

float get_brightness(int idx, float *leds_brightness){
  if ((idx < 0) || (idx >= NUM_LEDS)) {
    return leds_brightness[(idx + NUM_LEDS) % NUM_LEDS];
    // return 0;
  } else {
    return leds_brightness[idx];
  }
}

float apply_acc(float vel, float acc){
  float ret_vel = vel+acc;
  if (ret_vel > MAX_VEL){
    ret_vel = MAX_VEL;
  }
  if (ret_vel < -MAX_VEL){
    ret_vel = -MAX_VEL;
  }
  return ret_vel * VEL_DAMPING;
}

float next_for_idx(int idx, float *leds_brightness, float *leds_vel){
  float a = 0;
  float dist;
  float brightness;
  float force;
  // how hard does each spring pull on light `idx`?
  for (int i = 0; i < neighbor_count; i++){
    dist = neighbor_dist[i];
    brightness = (
      get_brightness(idx, leds_brightness) -
      get_brightness(idx + idx_offset[i], leds_brightness)
    );
    force = accel(brightness, dist);
    a -= force; // assume mass is 1.
  }
  leds_vel[idx] = apply_acc(leds_vel[idx], a);

  return leds_brightness[idx] + leds_vel[idx];
}

CRGB bright2CRGB(float brightness){
  if (brightness <= 0) {
    return CRGB(-brightness, 0, -brightness);
  } else if (brightness > 0) {
    return CRGB(0,brightness / 4,brightness / 2);
  } else {
    return CRGB(0,0,0);
  }
}

CRGB dualbright2CRGB(float abright, float bbright){
  float r=0, b=0, g=0;
  if (abright > 0) {
    r += abright / 2;
    b += abright;
  } else if (abright <= 0) {
    b -= 2 * abright / 3;
    g -= abright / 5;
  }
  if (bbright > 0) {
    r += bbright;
    b += bbright/3;
  } else if (bbright <= 0) {
    b -= abright / 2;
    g -= abright / 5;
  }
  return CRGB(r/2, g/2, 2 * b/3);
}

bool atoggle = true;
bool btoggle = true;
CRGB temp_led_val;

void rain() {
  for (int i = 0; i < NUM_LEDS; i++) {
    anext_leds_brightness[i] = next_for_idx(i, aleds_brightness, aleds_vel);
    bnext_leds_brightness[i] = next_for_idx(i, bleds_brightness, bleds_vel);
  }
  if (random(10) == 0){
    if (atoggle) anext_leds_brightness[random(NUM_LEDS)] = 60.0;
    else anext_leds_brightness[random(NUM_LEDS)] = - 60.0;
    atoggle = !atoggle;
  }
  if (random(10) == 0){
    if (btoggle) bnext_leds_brightness[random(NUM_LEDS)] = 60.0;
    else bnext_leds_brightness[random(NUM_LEDS)] = - 60.0;
    btoggle = !btoggle;
  }
  for (int k = 0; k<NUM_LEDS; k++){
    aleds_brightness[k] = anext_leds_brightness[k];
    bleds_brightness[k] = bnext_leds_brightness[k];
    temp_led_val = dualbright2CRGB(aleds_brightness[k], bleds_brightness[k]);
    leds[(L*(counter/FALL_SPEED)+k+46) % TRUE_LED_COUNT] = temp_led_val;
    leds[(L*(counter/FALL_SPEED)+k+46+NUM_LEDS) % TRUE_LED_COUNT] = temp_led_val;
  }
  show_at_max_brightness_for_power();
  counter++;
  counter %= TRUE_LED_COUNT * FALL_SPEED;
}

void loop(){
  rain();
}
