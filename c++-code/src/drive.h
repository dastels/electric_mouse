// Motor interface
//
// Copyright (c) 2020 Dave Astels

#ifndef __DRIVE_H__
#define __DRIVE_H__

#include "digital_output.h"
#include "motor.h"


typedef enum {STOPPED, FORWARD, REVERSE, PIVOT_LEFT, PIVOT_RIGHT, VEER_LEFT, VEER_RIGHT} DriveState;

// -----------------------------------------------------------------------------
// Abstract base

class Drive
{
  DriveState _state;

 public:
  Drive();
  DriveState state() { return _state; }
  virtual void enable() {}
  virtual void disable() {}
  virtual void stop() { _state = STOPPED; }
  virtual void forward(int pwm=255) { _state = FORWARD; }
  virtual void reverse(int pwm=255) { _state = REVERSE; }
  virtual void pivot_left(int pwm=128) { _state = PIVOT_LEFT; }
  virtual void pivot_right(int pwm=128) { _state = PIVOT_RIGHT; }
  virtual void veer_left() { _state = VEER_LEFT; }
  virtual void veer_right() { _state = VEER_RIGHT; }
};

// -----------------------------------------------------------------------------
// Fake implementation for desk debugging

class FakeDrive: public Drive
{
public:
  FakeDrive();
  virtual void stop();
  virtual void forward(int pwm=255);
  virtual void reverse(int pwm=255);
  virtual void pivot_left(int pwm=128);
  virtual void pivot_right(int pwm=128);
  virtual void veer_left();
  virtual void veer_right();
};


// -----------------------------------------------------------------------------
// Real implementation using the motors

class RealDrive: public Drive
{
  DigitalOutput motor_enable;
  Motor left;
  Motor right;

public:
  RealDrive();
  virtual void enable();
  virtual void disable();
  virtual void stop();
  virtual void forward(int pwm=255);
  virtual void reverse(int pwm=255);
  virtual void pivot_left(int pwm=128);
  virtual void pivot_right(int pwm=128);
  virtual void veer_left();
  virtual void veer_right();
};

#endif
