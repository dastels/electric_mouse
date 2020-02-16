// Motor interface
//
// Copyright (c) 2020 Dave Astels

#include <Arduino.h>
#include "drive.h"
#include "system.h"



// -----------------------------------------------------------------------------
// Abstract base

Drive::Drive(System *sys)
  : _state(DriveState::STOPPED)
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
  _state = DriveState::STOPPED;
}


void Drive::forward()
{
  _state = DriveState::FORWARD;
}


void Drive::reverse()
{
  _state = DriveState::REVERSE;
}


void Drive::pivot_left()
{
  _state = DriveState::PIVOT_LEFT;
}


void Drive::pivot_right()
{
  _state = DriveState::PIVOT_RIGHT;
}


void Drive::veer_left()
{
  _state = DriveState::VEER_LEFT;
}


void Drive::veer_right()
{
  _state = DriveState::VEER_RIGHT;
}


// -----------------------------------------------------------------------------
// Fake implementation for desk debugging

FakeDrive::FakeDrive(System *sys)
  :Drive(sys)
{
}


void FakeDrive::stop()
{
  Drive::stop();
}


void FakeDrive::forward()
{
  Drive::forward();

}


void FakeDrive::reverse()
{
  Drive::reverse();
}


void FakeDrive::pivot_left()
{
  Drive::pivot_left();
}


void FakeDrive::pivot_right()
{
  Drive::pivot_right();
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
  , _base_speed(128)
  , _half_speed( 64)

{
}


void RealDrive::base_speed(int speed)
{
  Drive::base_speed(speed);
  _base_speed = speed;
  _half_speed = speed / 2;
}


void RealDrive::trim(const int left_trim, const int right_trim)
{
  Drive::trim(left_trim, right_trim);
  _left->trim(left_trim);
  _right->trim(right_trim);
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


void RealDrive::forward()
{
  Drive::forward();
  _left->throttle(_base_speed);
  _right->throttle(_base_speed);
}


void RealDrive::reverse()
{
  Drive::reverse();
  _left->throttle(-_base_speed);
  _right->throttle(-_base_speed);
}


void RealDrive::pivot_left()
{
  Drive::pivot_left();
  _left->throttle(-_half_speed);
  _right->throttle(_half_speed);
}


void RealDrive::pivot_right()
{
  Drive::pivot_right();
  _left->throttle(_half_speed);
  _right->throttle(-_half_speed);
}


void RealDrive::veer_left()
{
  Drive::veer_left();
  _left->throttle(_half_speed);
  _right->throttle(_base_speed);
}


void RealDrive::veer_right()
{
  Drive::veer_right();
  _left->throttle(_base_speed);
  _right->throttle(_half_speed);
}
