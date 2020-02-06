// Motor interface
//
// Copyright (c) 2020 Dave Astels

#include <Arduino.h>
#include "drive.h"


// -----------------------------------------------------------------------------
// Abstract base

Drive::Drive(): _state(STOPPED)
{
}


// -----------------------------------------------------------------------------
// Fake implementation for desk debugging

FakeDrive::FakeDrive()
{
  Drive();
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

RealDrive::RealDrive()
  : motor_enable(DigitalOutput(A2))
  , left(Motor(13, 12))
  , right(Motor(11, 4))

{
  Drive();
}

void RealDrive::enable()
{
  motor_enable.write(true);
}


void RealDrive::disable()
{
  motor_enable.write(false);
}


void RealDrive::stop()
{
  Drive::stop();
  left.throttle(0);
  right.throttle(0);
}


void RealDrive::forward(int pwm)
{
  Drive::forward(pwm);
  left.throttle(pwm);
  right.throttle(pwm);
}


void RealDrive::reverse(int pwm)
{
  Drive::reverse(pwm);
  left.throttle(-pwm);
  right.throttle(-pwm);
}


void RealDrive::pivot_left(int pwm)
{
  Drive::pivot_left(pwm);
  left.throttle(-pwm);
  right.throttle(pwm);
}


void RealDrive::pivot_right(int pwm)
{
  Drive::pivot_right(pwm);
  left.throttle(pwm);
  right.throttle(-pwm);
}


void RealDrive::veer_left()
{
  Drive::veer_left();
  left.throttle(128);
  right.throttle(255);
}


void RealDrive::veer_right()
{
  Drive::veer_right();
  left.throttle(255);
  right.throttle(128);
}
