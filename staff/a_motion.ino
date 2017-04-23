#include <NXPMotionSense.h>
#include <Wire.h>
#include <EEPROM.h>
#include <util/crc16.h>

NXPMotionSense imu;
NXPSensorFusion filter;
float ax, ay, az;
float gx, gy, gz;
float mx, my, mz;
float roll, pitch, heading;

double lean;

void setupPropShield() {
  imu.begin();
  filter.begin(100);
}

struct CRGB motions[100];

void updateMotion() {
  // get and print uncalibrated data
  if (imu.available()) {
    // Read the motion sensors
    imu.readMotionSensor(ax, ay, az, gx, gy, gz, mx, my, mz);

    // Update the SensorFusion filter
    filter.update(gx, gy, gz, ax, ay, az, mx, my, mz);

    // print the heading, pitch and roll
    roll = filter.getRoll();
    pitch = filter.getPitch();
    heading = filter.getYaw();

    lean = abs((mx + my) / 80);
  }
}

