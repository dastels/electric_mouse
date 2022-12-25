// Motor interface
//
// Copyright (c) 2020 Dave Astels

#include <Arduino.h>
#include "motor.h"

Motor::Motor(int index, int pin_a, int pin_b, int enc_a_pin, int enc_b_pin)
  : _a(new AnalogOutput(pin_a))
  , _b(new AnalogOutput(pin_b))
  , _encoder_a(new DigitalInput(enc_a_pin))
  , _encoder_b(new DigitalInput(enc_b_pin))
{
  _a->write(0);
  _b->write(0);
  _motors[index] = this;
  attachInterrupt(digitalPinToInterrupt(enc_a_pin), index == 0 ? Motor::interrupt0: Motor::interrupt1, RISING);
}

void Motor::interrupt0()
{
  interrupt(_motors[0]);
}


void Motor::interrupt1()
{
  interrupt(_motors[1]);
}


void Motor::interrupt(Motor *m) {
  m->_direction = m->_encoder_b->value() ? MotorDirection::FORWARD : MotorDirection::BACKWARD;;

  uint32_t curr_a = micros();
  if (m->_last_a < curr_a) {
    // did not wrap around
    float rev = curr_a - m->_last_a;  // us
    rev = 1.0 / rev;            // rev per us
    rev *= 1000000;             // rev per sec
    rev *= 60;                  // rev per min
    rev /= GEARING;             // account for gear ratio
    rev /= ENCODERMULT;         // account for multiple ticks per rotation
    m->_rpm = rev;
  }
  m->_last_a = curr_a;
}

void Motor::throttle(int pwm)
{
  if (pwm == 0) {               // stop
    _a->write(0);
    _b->write(0);
  } else if (pwm < 0) {         // reverse
    _a->write(-pwm);
    _b->write(0);
  } else {                      // forward
    _a->write(0);
    _b->write(pwm);
  }
}
