// -*- mode: c++ -*-
// Digital input pin
//
// Copyright (c) 2020 Dave Astels

#ifndef __DIGITAL_INPUT_H__
#define __DIGITAL_INPUT_H__

#include "debouncable.h"

enum class Pull {NONE, UP, DOWN};

class DigitalInput: public Debouncable
{
  int _pin_number;
  int _mode;

 public:
  DigitalInput(int pin, Pull pull=Pull::UP);
  bool value();
};


#endif
