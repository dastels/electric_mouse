// Digital input pin
//
// Copyright (c) 2020 Dave Astels

#include <Arduino.h>
#include "digital_input.h"


DigitalInput::DigitalInput(int pin, Pull pull)
  : _pin_number(pin)
{
  switch (pull) {
  case Pull::NONE:
  case Pull::DOWN:
    pinMode(pin, INPUT);
    break;
  case Pull::UP:
    pinMode(pin, INPUT_PULLUP);
    break;
  }
}


bool DigitalInput::value()
{
  return digitalRead(_pin_number);
}
