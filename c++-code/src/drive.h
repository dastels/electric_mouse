// -*- mode: c++ -*-
// Motor interface
//
// Copyright (c) 2020 Dave Astels

#ifndef __DRIVE_H__
#define __DRIVE_H__

#include "digital_output.h"
#include "motor.h"

class System;

enum class DriveState {STOPPED, FORWARD, REVERSE, PIVOT_LEFT, PIVOT_RIGHT, VEER_LEFT, VEER_RIGHT};

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
  virtual void base_speed(int speed) {}
  virtual void trim(const int left_trim, const int right_trim) {}
  virtual void enable() {}
  virtual void disable() {}
  virtual void stop();
  virtual void forward();
  virtual void reverse();
  virtual void pivot_left();
  virtual void pivot_right();
  virtual void veer_left();
  virtual void veer_right();
};

// -----------------------------------------------------------------------------
// Fake implementation for desk debugging

class FakeDrive: public Drive
{
public:
  FakeDrive(System *sys);
  void stop();
  void forward();
  void reverse();
  void pivot_left();
  void pivot_right();
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
  int _base_speed;
  int _half_speed;

public:
  RealDrive(System *sys);
  void base_speed(int speed);
  void trim(const int left_trim, const int right_trim);
  void enable();
  void disable();
  void stop();
  void forward();
  void reverse();
  void pivot_left();
  void pivot_right();
  void veer_left();
  void veer_right();
};

#endif
