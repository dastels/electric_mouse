// -*- mode: c++ -*-
// Hardware interface
//
// Copyright (c) 2020 Dave Astels

#ifndef __SYSTEM_H__
#define __SYSTEM_H__

#include <Adafruit_NeoPixel.h>
#include "debouncer.h"
#include "digital_output.h"
#include "digital_input.h"

class Drive;

class System
{
  Adafruit_NeoPixel *_strip;
  Drive *_drive;
  DigitalOutput *_propwing_power;
  Debouncer *_right_whisker;
  Debouncer *_left_whisker;
  Debouncer *_low_voltage;
  Debouncer *_propwing_switch;



 public:
  System(bool debug_mode);
  void update(uint32_t now);
  void power_off();
  void power_on();
  Drive *drive() { return _drive; }
  Debouncer *right_whisker() { return _right_whisker; }
  Debouncer *left_whisker() { return _left_whisker; }
  Debouncer *low_voltage() { return _low_voltage; }
  Debouncer *propwing_switch() { return _propwing_switch; }
  void indicate(uint8_t red, uint8_t green, uint8_t blue);
};

#endif
