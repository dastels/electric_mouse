// -*- mode: c++ -*-
// Range monitoring arounf the robot
//
// Copyright (c) 2022 Dave Astels

#ifndef __RANGE_H__
#define __RANGE_H__

#include "tof.h"

typedef void (*RangeCallback)(Tof*);

typedef
class Range
{
  Tof *_tofs[8];
  RangeCallback _callback;
  uint8_t _callback_threshold = 0;

public:
  Range();
  void update();
  Tof *closest();
  float average_lux();
  uint8_t range(TofDirection dir);
  float lux(TofDirection dir);
  void register_callback(RangeCallback f, uint8_t threshold);
};

#endif
