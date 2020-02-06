// Digital output pin
//
// Copyright (c) 2020 Dave Astels

#include <Arduino.h>
#include "digital_output.h"

DigitalOutput::DigitalOutput(int pin)
  : _pin_number(pin)
  , _value(false)
{
  pinMode(pin, OUTPUT);
}


void DigitalOutput::write(bool val)
{
  _value = val;
  digitalWrite(_pin_number, val);
}


void DigitalOutput::set()
{
  write(true);
}


void DigitalOutput::clear()
{
  write(false);
}


void DigitalOutput::toggle()
{
  write(!_value);
}
