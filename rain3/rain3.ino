/*
simlated a mesh of springs, where brightness is the height each light
is away from the "mesh"
*/

#include "FastLED.h"
#define LED_DT 7              // Data pin to connect to the strip.
#define COLOR_ORDER GRB       // Are they RGB, GRB or what??
#define LED_TYPE WS2812B      // Don't forget to change LEDS.addLeds
#define TRUE_LED_COUNT 300 /// 1500 if this is a staff, 300 if it is a lamp.
// #define NUM_LEDS 150
#define BRIGHTNESS_GAIN 1.0
#define MAX_BRIGHTNESS 80

#define DROP_FORCE 30

#define TRANSITION_LENGTH 30
#define TRANSITION_DELAY 80
#define TRANSITION_LENGTH_VARIANCE 30
#define TRANSITION_DELAY_VARIANCE 300

#define L 8 // LIGHTS_PER_WRAP
#define O 2 // LOOP_OFFSET_MM
#define W 10 // HORIZONTAL_SPACE_MM
#define H 10 // VERTICAL_SPACE_MM
// #define K 3 // SPRING_CONST


#define VEL_DAMPING .9 // number between 0 and 1;
#define MAX_VEL 1.5
#define FALL_SPEED 0

/* non-default */
#define NUM_LEDS 100 //1500
#define K 1 // SPRING_CONST
// #define DROP_FORCE 60
// #define MAX_BRIGHTNESS 128
// #define BRIGHTNESS_GAIN 1.0
/* end non-default */


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

float color_rotate = 0.0;
float color_a[6];
float color_b[6];

void change_color(float *color_array){
  for (int i = 0; i < 6; i++)
    color_array[i] = ((float)random(255)) / 255.0;
  Serial.printf("\n%.2f %.2f %.2f\n%.2f %.2f %.2f",
    color_array[0],
    color_array[1],
    color_array[2],
    color_array[3],
    color_array[4],
    color_array[5]
  );
  Serial.println();
}

int color_transition = 1;
int color_transition_len = 0;
int color_transition_delay = 20;
float color_transition_inc = 0.0;

void transition(){
  if (color_transition-- <= 0){
    if (color_transition_len > 0){
      color_transition_len = 0;
      color_transition = random(TRANSITION_DELAY_VARIANCE) + TRANSITION_DELAY;
      color_transition_delay = color_transition;
    } else {
      color_transition_delay = 0;
      color_transition = random(TRANSITION_LENGTH_VARIANCE) + TRANSITION_LENGTH;
      color_transition_len = color_transition;
      if (color_rotate < .1){
        change_color(color_a);
        color_transition_inc = (1-color_rotate) / color_transition_len;
      } else {
        change_color(color_b);
        color_transition_inc = -(color_rotate / color_transition_len);
      }
    }
    Serial.println();
  } else {
    if (color_transition_len > 0){
      color_rotate += color_transition_inc;
    }
    if (color_transition_delay > 0){
      // waiting
    }
  }
}

CRGB dualbright2CRGB(float abright, float bbright){
  float r=0, b=0, g=0;
  if (abright > 0) {
    r += color_rotate * abright * color_a[0];
    g += color_rotate * abright * color_a[1];
    b += color_rotate * abright * color_a[2];
  } else if (abright <= 0) {
    r -= color_rotate * abright * color_a[3];
    g -= color_rotate * abright * color_a[4];
    b -= color_rotate * abright * color_a[5];
  }
  if (bbright > 0) {
    r += (1 - color_rotate) * bbright * color_b[0];
    g += (1 - color_rotate) * bbright * color_b[1];
    b += (1 - color_rotate) * bbright * color_b[2];
  } else if (bbright <= 0) {
    r -= (1 - color_rotate) * bbright * color_b[3];
    g -= (1 - color_rotate) * bbright * color_b[4];
    b -= (1 - color_rotate) * bbright * color_b[5];
  }
  return CRGB(
    min(BRIGHTNESS_GAIN * r, MAX_BRIGHTNESS),
    min(BRIGHTNESS_GAIN * g, MAX_BRIGHTNESS),
    min(BRIGHTNESS_GAIN * 4 * b/3, MAX_BRIGHTNESS)
  );
}

bool atoggle = true;
bool btoggle = true;
CRGB temp_led_val;

float drop_force;

void rain() {
  transition();
  for (int i = 0; i < NUM_LEDS; i++) {
    anext_leds_brightness[i] = next_for_idx(i, aleds_brightness, aleds_vel);
    bnext_leds_brightness[i] = next_for_idx(i, bleds_brightness, bleds_vel);
  }
  if (random(10) == 0){
    drop_force = random(10) + DROP_FORCE;
    if (atoggle) anext_leds_brightness[random(NUM_LEDS)] = drop_force;
    else anext_leds_brightness[random(NUM_LEDS)] = -drop_force;
    atoggle = !atoggle;
  }
  if (random(10) == 0){
    drop_force = random(10) + DROP_FORCE;
    if (btoggle) bnext_leds_brightness[random(NUM_LEDS)] = drop_force;
    else bnext_leds_brightness[random(NUM_LEDS)] = -drop_force;
    btoggle = !btoggle;
  }
  for (int _k = 0, k=0; _k<TRUE_LED_COUNT; _k++){
    k =  _k%NUM_LEDS;
    k = 20 / NUM_LEDS % 2 == 0 ? k : (NUM_LEDS - k);
    aleds_brightness[k] = anext_leds_brightness[k];
    bleds_brightness[k] = bnext_leds_brightness[k];
    temp_led_val = dualbright2CRGB(aleds_brightness[k], bleds_brightness[k]);
    leds[_k] = temp_led_val;
    leds[_k] = temp_led_val;
  }
  show_at_max_brightness_for_power();
  counter++;
  counter = counter % (TRUE_LED_COUNT * FALL_SPEED * 10);
}

void loop(){
  rain();
}
