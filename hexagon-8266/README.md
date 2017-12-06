# Necessary libraries

For both ESP32 and ESP8266, the following packages are needed:

* ArduinoJSON
  * https://github.com/bblanchon/ArduinoJson
  * Install from the Arduino IDE by going to Sketch->Library Manager then installing ArduinoJson by Benoit Blanchon

Additional packages are needed for the ESP32, as shown below.

# ESP32 support

As of 11/16/2017, the official espressif-esp32 libraries do not support the following ESP8266 equivalent libraries:

1. WebServer
1. WiFiManager
1. DNSServer

To install functional versions of those libraries, download the following:

* WebServer
  * https://github.com/carbocation/WebServer_tng
  * Didn't work: https://github.com/zhouhan0126/WebServer-esp32
* WiFiManager
  * Be sure to checkout the ESP32 branch
  * https://github.com/bbx10/WiFiManager/tree/esp32
  * Didn't work: https://github.com/zhouhan0126/WIFIMANAGER-ESP32
* DNSServer
  * https://github.com/bbx10/DNSServer_tng
  * Didn't work: https://github.com/zhouhan0126/DNSServer---esp32

Once these are downloaded:

* Download, extract, and copy these to the expressif/esp32 "libraries" folder.
* Restart Arduino before trying to build/upload your sketch to your ESP board.

Annoyingly, you need to manually go to 192.168.4.1 once you connect to the Soulmate Config ESP WiFi, since the captive portal isn't yet working correctly from a DNS perspective on ESP32.