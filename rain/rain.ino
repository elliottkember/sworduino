/*
simlated a mesh of springs, where brightness is the height each light
is away from the "mesh"
*/

#include "FastLED.h"
#define LED_DT 7              // Data pin to connect to the strip.
#define COLOR_ORDER GRB       // Are they RGB, GRB or what??
#define LED_TYPE WS2812B      // Don't forget to change LEDS.addLeds
#define NUM_LEDS 300 //1500

#define L 8 // LIGHTS_PER_WRAP
#define O 0 // LOOP_OFFSET_MM
#define W 10 // HORIZONTAL_SPACE_MM
#define H 10 // VERTICAL_SPACE_MM

#define K 2 // SPRING_CONST

#define VEL_DAMPING .9 // number between 0 and 1;
#define MAX_VEL 3


struct CRGB leds[NUM_LEDS];
float leds_vel[NUM_LEDS];
float leds_brightness[NUM_LEDS];
float next_leds_brightness[NUM_LEDS];

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
  LEDS.addLeds<LED_TYPE, LED_DT, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(255);
  set_max_power_in_volts_and_milliamps(5, 10000);
  randomSeed(analogRead(0));
  init_helper_values();

  for (int i = 0; i < NUM_LEDS; i++) {
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

float get_brightness(int idx){
  if ((idx < 0) || (idx >= NUM_LEDS)) {
    return 0;
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

float next_for_idx(int idx){
  float a = 0;
  float dist;
  float brightness;
  float force;
  if (idx == 100) Serial.printf("step %d", counter, leds_brightness[100]);
  // how hard does each spring pull on light `idx`?
  for (int i = 0; i < neighbor_count; i++){
    dist = neighbor_dist[i];
    brightness = get_brightness(idx) - get_brightness(idx + idx_offset[i]);
    force = accel(brightness, dist);
    a -= force; // assume mass is 1.
    //if (idx == 100) Serial.printf("\n-%d\td%.1f\tb%.1f\tf%.1f", i, dist, get_brightness(idx + idx_offset[i]), force);
  }
  if (idx == 100){
    Serial.printf("\n\tb%.1f\tv%.1f\ta%.1f", leds_brightness[100], leds_vel[100], a);
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

bool toggle = true;

void rain() {
  for (int i = 0; i < NUM_LEDS; i++) {
    next_leds_brightness[i] = next_for_idx(i);
  }
  Serial.println();
  if (random(10) == 0){
    if (toggle) next_leds_brightness[random(NUM_LEDS)] = 30.0;
    else next_leds_brightness[random(NUM_LEDS)] = - 30.0;
    toggle = !toggle;
  }
  for (int k = 0; k<NUM_LEDS; k++){
    leds_brightness[k] = next_leds_brightness[k];
    leds[k] = bright2CRGB(leds_brightness[k]);
  }
  show_at_max_brightness_for_power();
  counter++;
}

void loop(){
  //EVERY_N_MILLISECONDS(10) {
    rain();
  //}
}


/*
int neighbor_count = 24;
float neighbor_dist[24];
int idx_offset[24];

float pythag(float w, float h){
  return sqrt((h*h)+(w*w));
}

void init_helper_values() {
  neighbor_dist[0] = pythag(2*W + 2*O, 2*H);
  neighbor_dist[1] = pythag(1*W + 2*O, 2*H);
  neighbor_dist[2] = pythag(0*W + 2*O, 2*H);
  neighbor_dist[3] = pythag(1*W - 2*O, 2*H);
  neighbor_dist[4] = pythag(2*W - 2*O, 2*H);

  neighbor_dist[5] = pythag(2*W + O, H);
  neighbor_dist[6] = pythag(1*W + O, H);
  neighbor_dist[7] = pythag(0*W + O, H);
  neighbor_dist[8] = pythag(1*W - O, H);
  neighbor_dist[9] = pythag(2*W - O, H);

  neighbor_dist[10] = pythag(2*W + 0, 0);
  neighbor_dist[11] = pythag(1*W + 0, 0);
  neighbor_dist[12] = pythag(1*W - 0, 0);
  neighbor_dist[13] = pythag(2*W - 0, 0);

  neighbor_dist[14] = neighbor_dist[9];
  neighbor_dist[15] = neighbor_dist[8];
  neighbor_dist[16] = neighbor_dist[7];
  neighbor_dist[17] = neighbor_dist[6];
  neighbor_dist[18] = neighbor_dist[5];

  neighbor_dist[19] = neighbor_dist[4];
  neighbor_dist[20] = neighbor_dist[3];
  neighbor_dist[21] = neighbor_dist[2];
  neighbor_dist[22] = neighbor_dist[1];
  neighbor_dist[23] = neighbor_dist[0];

  idx_offset[0] = -2*L - 2;
  idx_offset[1] = -2*L - 1;
  idx_offset[2] = -2*L - 0;
  idx_offset[3] = -2*L + 1;
  idx_offset[4] = -2*L + 2;

  idx_offset[5] = -1*L - 2;
  idx_offset[6] = -1*L - 1;
  idx_offset[7] = -1*L - 0;
  idx_offset[8] = -1*L + 1;
  idx_offset[9] = -1*L + 2;

  idx_offset[10] = -2;
  idx_offset[11] = -1;
  idx_offset[12] = 1;
  idx_offset[13] = 2;

  idx_offset[14] = L - 2;
  idx_offset[15] = L - 1;
  idx_offset[16] = L - 0;
  idx_offset[17] = L + 1;
  idx_offset[18] = L + 2;

  idx_offset[19] = 2*L - 2;;
  idx_offset[20] = 2*L - 1;;
  idx_offset[21] = 2*L - 0;;
  idx_offset[22] = 2*L + 1;;
  idx_offset[23] = 2*L + 2;;
}
*/