// -*- mode: c++ -*-
// Motor interface
//
// Copyright (c) 2020 Dave Astels

#ifndef __DRIVE_H__
#define __DRIVE_H__

#include "digital_output.h"
#include "motor.h"

class System;

typedef enum {STOPPED, FORWARD, REVERSE, PIVOT_LEFT, PIVOT_RIGHT, VEER_LEFT, VEER_RIGHT} DriveState;

// -----------------------------------------------------------------------------
// Abstract base

class Drive
{
protected:
  DriveState _state;
  System *_system;

public:
  static Drive *make_drive(bool debug_mode, System *sys);
  Drive(System *sys);
  DriveState state() { return _state; }
  virtual void enable() = 0;
  virtual void disable() = 0;
  virtual void stop();
  virtual void forward(int pwm=255);
  virtual void reverse(int pwm=255);
  virtual void pivot_left(int pwm=128);
  virtual void pivot_right(int pwm=128);
  virtual void veer_left();
  virtual void veer_right();
};

// -----------------------------------------------------------------------------
// Fake implementation for desk debugging

class FakeDrive: public Drive
{
public:
  FakeDrive(System *sys);
  void enable();
  void disable();
  void stop();
  void forward(int pwm=255);
  void reverse(int pwm=255);
  void pivot_left(int pwm=128);
  void pivot_right(int pwm=128);
  void veer_left();
  void veer_right();
};


// -----------------------------------------------------------------------------
// Real implementation using the motors

class RealDrive: public Drive
{
  DigitalOutput *_motor_enable;
  Motor *_left;
  Motor *_right;

public:
  RealDrive(System *sys);
  void enable();
  void disable();
  void stop();
  void forward(int pwm=255);
  void reverse(int pwm=255);
  void pivot_left(int pwm=128);
  void pivot_right(int pwm=128);
  void veer_left();
  void veer_right();
};

#endif
