// Analog output pin
//
// Copyright (c) 2020 Dave Astels

#include <Arduino.h>
#include "analog_output.h"

AnalogOutput::AnalogOutput(int pin)
  : _pin_number(pin)
  , _value(0)
{
  pinMode(pin, OUTPUT);
}


void AnalogOutput::write(uint8_t val)
{
  _value = val;
  analogWrite(_pin_number, val);
}
