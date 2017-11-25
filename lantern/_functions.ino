// Godawful curve function
bool increasing = true;
int curve(int value, int difference, int maximum) {
  if (value < maximum && increasing) {
    value += difference;
  } else if (value > 1) {
    value -= difference;
    increasing = false;
  } else {
    increasing = true;
    value += difference;
  }
  return value;
}
