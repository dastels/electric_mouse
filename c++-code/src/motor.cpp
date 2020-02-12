// Motor interface
//
// Copyright (c) 2020 Dave Astels


#include "motor.h"

Motor::Motor(int pin_a, int pin_b)
  : _a(new AnalogOutput(pin_a))
  , _b(new AnalogOutput(pin_b))

{
  _a->write(0);
  _b->write(0);
}


void Motor::throttle(int pwm)
{
  if (pwm < 0) {  // reverse speeds
    _a->write(-pwm);
    _b->write(0);

  } else { // stop or forward
    _a->write(0);
    _b->write(pwm);
  }
}
