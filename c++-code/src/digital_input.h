// Digital input pin
//
// Copyright (c) 2020 Dave Astels

#ifndef __DIGITAL_INPUT_H__
#define __DIGITAL_INPUT_H__

#include "debouncable.h"

typedef enum {NONE, UP, DOWN} Pull;

class DigitalInput: public Debouncable
{
  int _pin_number;
  int _mode;

 public:
  DigitalInput(int pin, Pull pull=UP);
  bool value();
};


#endif
