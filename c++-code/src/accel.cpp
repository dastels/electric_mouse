// -*- mode: c++ -*-
// Accelerometer interface
//
// Copyright (c) 2020 Dave Astels

#include <Adafruit_Sensor.h>
#include "accel.h"
#include "logging.h"

extern Logger *logger;
void critical_failure(const char *msg);

Accel::Accel()
  :_mpu(new Adafruit_MPU6050())
  ,_accel_x(0.0)
  ,_accel_y(0.0)
  ,_accel_z(0.0)
  ,_gyro_x(0.0)
  ,_gyro_y(0.0)
  ,_gyro_z(0.0)
  ,_temperature(0.0)

{
  if (!_mpu->begin()) {
    critical_failure("Failed to initialize MPU6050");
  }
  _mpu->setAccelerometerRange(MPU6050_RANGE_8_G);
  _mpu->setGyroRange(MPU6050_RANGE_500_DEG);
}


void Accel::update()
{
  sensors_event_t a, g, temp;
  _mpu->getEvent(&a, &g, &temp);
  _accel_x = a.acceleration.x;
  _accel_y = a.acceleration.y;
  _accel_z = a.acceleration.z;
  _gyro_x = g.gyro.x;
  _gyro_y = g.gyro.y;
  _gyro_z = g.gyro.z;
  _temperature = temp.temperature;
}
