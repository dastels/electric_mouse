// -*- mode: c++ -*-
// Accelerometer interface
//
// Copyright (c) 2022 Dave Astels

#ifndef __ACCEL_H__
#define __ACCEL_H__

#include <Adafruit_MPU6050.h>

class Accel
{
  Adafruit_MPU6050 *_mpu;
  float _accel_x;
  float _accel_y;
  float _accel_z;
  float _gyro_x;
  float _gyro_y;
  float _gyro_z;
  float _temperature;
public:
  Accel();
  void update();
};

#endif __ACCEL_H__
