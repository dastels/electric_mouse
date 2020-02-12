// -*- mode: c++ -*-
// Digital output pin
//
// Copyright (c) 2020 Dave Astels

#ifndef __DIGITAL_OUTPUT_H__
#define __DIGITAL_OUTPUT_H__


class DigitalOutput
{
  int _pin_number;
  bool _value;

 public:
  DigitalOutput(int pin);
  void write(bool val);
  void set();
  void clear();
  void toggle();
  bool value() { return _value; }
};

#endif
