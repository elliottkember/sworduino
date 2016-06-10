# The Staff

This code runs on Elliott's LED light staff, JOY. Each pattern is a function that modifies the "leds" array of CRGB values. 

- I'm using the [FastLED](https://github.com/FastLED/FastLED) library to run the patterns.
- I'm also using a Teensy board, so you'll need [Teensyduino](https://www.pjrc.com/teensy/teensyduino.html) to upload the code.
- Staff patterns are in staff/staff.ino

![](http://i.giphy.com/YRDnMd32QNujS.gif)

*TODO:*
- [ ] It'd be interesting if each pattern took an array of the existing LEDs and a frame number, and returned a new array.
- [ ] Stein's patterns needs to be merged into the main staff code
