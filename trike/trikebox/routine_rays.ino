#define NUMBER_OF_RAYS 10

namespace Rays {
  
  class Point {
  public:
    double x, y;
    bool isGoingDownwards;
    int pointNumber;
    void reset() {
      if (isGoingDownwards) {
        y = -30;
        x = random(0, 72);
      } else {
        x = -50; // random(0, 100) > 50 ? 80 : -10;
        y = random(0, 12);
      }
    }
    void start() {
      isGoingDownwards = rand() % 2 == 0;
      x = random(0, 72);
      y = random(0, 16);
    }
    void move() {
      if (isGoingDownwards) {
        y += (double)beatsin16(30, 4, 10, pointNumber * 3000) / 100; 
      } else {
        x += (double)beatsin16(30, 10, 50, pointNumber * 3000) / 100;
      }
      if (y > 30 || x > 120) {
        reset();
      }
    }
    void draw() {
      int diameter = beatsin16(30, 3, 10, pointNumber * 3000, pointNumber * 3000);
      
      for (uint16_t xx = x - diameter; xx < x + diameter; xx++) {
        for (uint16_t yy = y - diameter; yy < y + diameter; yy++) {
          uint16_t index = yy * 72 + xx;
          double distance = sqrt16(sq(xx - x) + sq(yy - y));
          if (index > 0 && index < N_LEDS && distance < diameter) {
            Soulmate::led_arr[index] += CHSV(pointNumber * 30, 255, 255 / distance);
          }
        }
      }
    }
  };
  
  Point drops[NUMBER_OF_RAYS] = {};
  bool initializedPoints = false;
  
  void draw() {
    if (!initializedPoints) {
      for (int i = 0; i < NUMBER_OF_RAYS; i++) {
        Point p = drops[i];
        p.pointNumber = i;
        p.start();
        drops[i] = p;
      }
      initializedPoints = true;
    }
    
    for (int p = 0; p < NUMBER_OF_RAYS; p++) {
      Point point = drops[p];
      point.move();
      drops[p] = point;
    }
  
    fadeToBlackBy(Soulmate::led_arr, N_LEDS, 120);

    for (int p = 0; p < NUMBER_OF_RAYS; p++) {
      drops[p].draw();
    }
  }
}

void rays() {
  Rays::draw();
}

