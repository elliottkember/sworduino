#include <NXPMotionSense.h>
#include <Wire.h>
#include <EEPROM.h>
NXPMotionSense imu;
NXPSensorFusion filter;
float ax, ay, az;
float gx, gy, gz;
float mx, my, mz;
float roll, pitch, heading;

bool isMotionEnabled() {
  return false;
}

void setupMotion(){ 
  if (isMotionEnabled()) {
    imu.begin();
    filter.begin(100);
  }
}

double lean() {
  if (!isMotionEnabled()) {
    return false;
  }
  
  double x = ax; //mx / 45;
  double y = ay; //my / 45;
  double z = az; // (mz + 35) / 35;
  double lean = (x + y) / 2;
  return lean;
}

void readMotionIfAvailable() {
  if (!isMotionEnabled()) {
    return;
  }
  
  if (imu.available()) {
    // Read the motion sensors
    imu.readMotionSensor(ax, ay, az, gx, gy, gz, mx, my, mz);

    // Update the SensorFusion filter
    filter.update(gx, gy, gz, ax, ay, az, mx, my, mz);

    // print the heading, pitch and roll
    roll = filter.getRoll();
    pitch = filter.getPitch();
    heading = filter.getYaw();
  }
}

