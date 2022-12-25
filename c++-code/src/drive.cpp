// Motor interface
//
// Copyright (c) 2020 Dave Astels

#include <Arduino.h>
#include "logging.h"
#include "drive.h"
#include "system.h"

extern Logger *logger;


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
  if (_state == DriveState::STOPPED) {
    return;
  }
  _state = DriveState::STOPPED;
  logger->debug("Drive stopped");
}


void Drive::forward()
{
  if (_state == DriveState::FORWARD) {
    return;
  }
  _state = DriveState::FORWARD;
  logger->debug("Drive forward");
}


void Drive::reverse()
{
  if (_state == DriveState::REVERSE) {
    return;
  }
  _state = DriveState::REVERSE;
  logger->debug("Drive reverse");
}


void Drive::pivot_left()
{
  if (_state == DriveState::PIVOT_LEFT) {
    return;
  }
  _state = DriveState::PIVOT_LEFT;
  logger->debug("Drive pivot left");
}


void Drive::pivot_right()
{
  if (_state == DriveState::PIVOT_RIGHT) {
    return;
  }
  _state = DriveState::PIVOT_RIGHT;
  logger->debug("Drive pivot right");
}


void Drive::veer_left()
{
  if (_state == DriveState::VEER_LEFT) {
    return;
  }
  _state = DriveState::VEER_LEFT;
  logger->debug("Drive veer left");
}


void Drive::veer_right()
{
  if (_state == DriveState::VEER_RIGHT) {
    return;
  }
  _state = DriveState::VEER_RIGHT;
  logger->debug("Drive veer right");
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
  , _left(new Motor(0, 8, 9, 7, 28)) // motor 1, encoder on Grove 7
  , _right(new Motor(1, 10, 11, 0, 1)) // motor 2, encoder on Grove 1
  , _base_speed(128)
  , _half_speed(64)
  , _indicator(sys->indicator())
{
}


void RealDrive::base_speed(int speed)
{
  Drive::base_speed(speed);
  _base_speed = speed;
  _half_speed = speed / 2;
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
  _indicator->left_drive(0);
  _indicator->right_drive(0);
}


void RealDrive::forward()
{
  Drive::forward();
  _left->throttle(_base_speed);
  _right->throttle(_base_speed);
  _indicator->left_drive(_base_speed);
  _indicator->right_drive(_base_speed);
}


void RealDrive::reverse()
{
  Drive::reverse();
  _left->throttle(-_base_speed);
  _right->throttle(-_base_speed);
  _indicator->left_drive(-_base_speed);
  _indicator->right_drive(-_base_speed);
}


void RealDrive::pivot_left()
{
  Drive::pivot_left();
  _left->throttle(-_half_speed);
  _right->throttle(_half_speed);
  _indicator->left_drive(-_half_speed);
  _indicator->right_drive(_half_speed);
}


void RealDrive::pivot_right()
{
  Drive::pivot_right();
  _left->throttle(_half_speed);
  _right->throttle(-_half_speed);
  _indicator->left_drive(_half_speed);
  _indicator->right_drive(-_half_speed);
}


void RealDrive::veer_left()
{
  Drive::veer_left();
  _left->throttle(_half_speed);
  _right->throttle(_base_speed);
  _indicator->left_drive(_half_speed);
  _indicator->right_drive(_base_speed);
}


void RealDrive::veer_right()
{
  Drive::veer_right();
  _left->throttle(_base_speed);
  _right->throttle(_half_speed);
  _indicator->left_drive(_base_speed);
  _indicator->right_drive(_half_speed);
}
