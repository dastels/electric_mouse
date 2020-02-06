// Analog output pin
//
// Copyright (c) 2020 Dave Astels

#ifndef __ANALOG_OUTPUT_H__
#define __ANALOG_OUTPUT_H__

#include <stdint.h>


class AnalogOutput
{
  int _pin_number;
  uint8_t _value;

 public:
  AnalogOutput(int pin);
  void write(uint8_t val);
  uint8_t value() { return _value; }
};

#endif
