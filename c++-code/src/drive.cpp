// Motor interface
//
// Copyright (c) 2020 Dave Astels

#include <Arduino.h>
#include "drive.h"
#include "system.h"



// -----------------------------------------------------------------------------
// Abstract base

Drive::Drive(System *sys)
  : _state(STOPPED)
  , _system(sys)
{
}


Drive *Drive::make_drive(bool debug_mode, System *sys)
{
  if (debug_mode) {
    return new FakeDrive(sys);
  } else {
    return new RealDrive(sys);
  }
}


void Drive::stop()
{
  _state = STOPPED;
}


void Drive::forward(int pwm)
{
  _state = FORWARD;
}


void Drive::reverse(int pwm)
{
  _state = REVERSE;
}


void Drive::pivot_left(int pwm)
{
  _state = PIVOT_LEFT;
}


void Drive::pivot_right(int pwm)
{
  _state = PIVOT_RIGHT;
}


void Drive::veer_left()
{
  _state = VEER_LEFT;
}


void Drive::veer_right()
{
  _state = VEER_RIGHT;
}


// -----------------------------------------------------------------------------
// Fake implementation for desk debugging

FakeDrive::FakeDrive(System *sys)
  :Drive(sys)
{
}


void FakeDrive::enable()
{
}


void FakeDrive::disable()
{
}




void FakeDrive::stop()
{
  Drive::stop();
}


void FakeDrive::forward(int pwm)
{
  Drive::forward(pwm);

}


void FakeDrive::reverse(int pwm)
{
  Drive::reverse(pwm);
}


void FakeDrive::pivot_left(int pwm)
{
  Drive::pivot_left(pwm);
}


void FakeDrive::pivot_right(int pwm)
{
  Drive::pivot_right(pwm);
}


void FakeDrive::veer_left()
{
  Drive::veer_left();
}


void FakeDrive::veer_right()
{
  Drive::veer_right();
}


// -----------------------------------------------------------------------------
// Real implementation using the motors

RealDrive::RealDrive(System *sys)
  : Drive(sys)
  , _motor_enable(new DigitalOutput(16))
  , _left(new Motor(13, 12))
  , _right(new Motor(11, 4))

{
}

void RealDrive::enable()
{
  _motor_enable->set();
}


void RealDrive::disable()
{
  _motor_enable->clear();
}


void RealDrive::stop()
{
  Drive::stop();
  _left->throttle(0);
  _right->throttle(0);
}


void RealDrive::forward(int pwm)
{
  Drive::forward(pwm);
  _left->throttle(pwm);
  _right->throttle(pwm);
}


void RealDrive::reverse(int pwm)
{
  Drive::reverse(pwm);
  _left->throttle(-pwm);
  _right->throttle(-pwm);
}


void RealDrive::pivot_left(int pwm)
{
  Drive::pivot_left(pwm);
  _left->throttle(-pwm);
  _right->throttle(pwm);
}


void RealDrive::pivot_right(int pwm)
{
  Drive::pivot_right(pwm);
  _left->throttle(pwm);
  _right->throttle(-pwm);
}


void RealDrive::veer_left()
{
  Drive::veer_left();
  _left->throttle(128);
  _right->throttle(255);
}


void RealDrive::veer_right()
{
  Drive::veer_right();
  _left->throttle(255);
  _right->throttle(128);
}
