// Digital unsigned subtraction macro. if result <0, then => 0. Otherwise, take on fixed value.
#define qsubd(x, b)  ((x>b)?255:0)
// Analog Unsigned subtraction macro. if result <0, then => 0
#define qsuba(x, b)  ((x>b)?x-b:0)

#define LED_DT 7
#define COLOR_ORDER GRB
#define LED_TYPE WS2812B
#define VOLTS 5
#define MAX_CURRENT_IN_MA 10000
#define NUM_LEDS 300
#define CIRCUMFERENCE 8
#define ROTATION_IN_MS 3000
