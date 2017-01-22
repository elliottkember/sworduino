namespace UpAndDownBy {

  bool increasing = true;
  // TODO increasing and numberOfSparkles should probably be passed by reference here!
  int go(int value, int difference, int max, int min) {
    if (value < max && increasing) {
      value += difference;
    } else if (value > min) {
      value -= difference;
      increasing = false;
    } else {
      increasing = true;
      value += difference;
    }
    return value;
  }
}

int upAndDownBy(int value, int difference, int max, int min) {
  return UpAndDownBy::go(value, difference, max, min);
}
