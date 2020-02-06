// Motor interface
//
// Copyright (c) 2020 Dave Astels


#include "motor.h"

Motor::Motor(int pin_a, int pin_b)
  : a(AnalogOutput(pin_a))
  , b(AnalogOutput(pin_b))

{
  a.write(0);
  b.write(0);
}


void Motor::throttle(int pwm)
{
  if (pwm < 0) {  // reverse speeds
    a.write(-pwm);
    b.write(0);

  } else { // stop or forward
    a.write(0);
    b.write(pwm);
  }
}
