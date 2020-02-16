// Motor interface
//
// Copyright (c) 2020 Dave Astels


#include "motor.h"

Motor::Motor(int pin_a, int pin_b)
  : _a(new AnalogOutput(pin_a))
  , _b(new AnalogOutput(pin_b))
  , _trim(0)
{
  _a->write(0);
  _b->write(0);
}


void Motor::trim(const int t)
{
  _trim = t;
}


void Motor::throttle(int pwm)
{
  if (pwm == 0) {               // stop
    _a->write(0);
    _b->write(0);
  } else if (pwm < 0) {         // reverse
    _a->write(-pwm + _trim);
    _b->write(0);
  } else {                      // forward
    _a->write(0);
    _b->write(pwm + _trim);
  }
}
