// -*- mode: c++ -*-
// Motor interface
//
// Copyright (c) 2020 Dave Astels

#ifndef __MOTOR_H__
#define __MOTOR_H__

#include "analog_output.h"
#include "digital_input.h"

// These let us convert ticks-to-RPM
#define GEARING     (20)
#define ENCODERMULT (12)

enum class  MotorDirection {BACKWARD, FORWARD};


class Motor
{
  AnalogOutput *_a;
  AnalogOutput *_b;
  DigitalInput *_encoder_a;
  DigitalInput *_encoder_b;
  volatile float _rpm = 0;
  volatile uint32_t _last_a = 0;
  volatile MotorDirection _direction = MotorDirection::FORWARD;

  inline static Motor *_motors[2];
  static void interrupt0();
  static void interrupt1();
  static void interrupt(Motor *m);

 public:
  Motor(int index, int pin_a, int pin_b, int _enc_a_pin, int _enc_b_pin);
  void throttle(int pwm);
  float rpm() { return _rpm; }
  MotorDirection direction() { return _direction; }
};


#endif
