#define NUMBER_OF_RAYS 5

namespace Rays {

  class Point {
  public:
    double x, y;
    void reset() {
      y = -5;
      x = random(0, 72);
    }
    void start() {
      x = random(0, 72);
      y = random(0, 16);
    }
    void move(int p) {
      y += (double)beatsin16(12, 10, 50, p * 3000) / 100;
    }
  };
  
  Point drops[NUMBER_OF_RAYS] = {};
  bool initializedPoints = false;
  
  void draw() {
    if (!initializedPoints) {
      for (int i = 0; i < NUMBER_OF_RAYS; i++) {
        Point p = drops[i];
        p.start();
        drops[i] = p;
      }
      initializedPoints = true;
    }
    
    for (int p = 0; p < 5; p++) {
      Point point = drops[p];
      point.move(p);
      if (point.y > 20) {
        point.reset();
      }
      drops[p] = point;
    }
  
    fadeToBlackBy(Soulmate::led_arr, N_LEDS, 120);
      
    for (int y = 12; y >= 0; y--) {
      for (int x = 0; x < 72; x++) {
        uint16_t index = y * 72 + x;
        
        for (int p = 0; p < NUMBER_OF_RAYS; p++) {
          Point point = drops[p];
          if (abs8(point.x - x) < 10 && abs8(point.y - y) < 10) {
            double distance = sqrt16(sq(point.x - x) + sq(point.y - y));
            int diameter = beatsin16(30, 3, 10, p * 3000, p * 3000);
            if (distance < diameter) {
              Soulmate::led_arr[index] += CHSV(p * 30, 255, 255 / distance);
            }  
          }
        }
      }
    }
    
    map();
  }
}

void rays() {
  Rays::draw();
}

