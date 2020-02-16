// -*- mode: c++ -*-
// Motor interface
//
// Copyright (c) 2020 Dave Astels

#ifndef __MOTOR_H__
#define __MOTOR_H__

#include "analog_output.h"


class Motor
{
  AnalogOutput *_a;
  AnalogOutput *_b;
  int _trim;

 public:
  Motor(int pin_a, int pin_b);
  void trim(const int t);
  void throttle(int pwm);
};


#endif
