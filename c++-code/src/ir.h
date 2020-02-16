// -*- mode: c++ -*-
// Hardware interface
//
// Copyright (c) 2020 Dave Astels

#ifndef __IR_H__
#define __IR_H__

#include <Adafruit_MLX90640.h>
#include "event.h"

enum class IrSlice {ALL, LEFT, CENTER, RIGHT};

class Ir
{
  float _frame[768];
  int _left_width;
  int _right_width;
  Adafruit_MLX90640 *_sensor;
  float _threshold;

  int start_of(IrSlice slice);
  int end_of(IrSlice slice);
  int width_of(IrSlice slice);

public:
  Ir(float threshold=22.0);
  void threshold(float th);
  int pixels_in(IrSlice slice);
  float average(IrSlice slice);
  float hottest(IrSlice slice);
  float coldest(IrSlice slice);
  int percent_above(IrSlice slice);
  void split(int left_width, int right_width);
  EventFocus focus();
  void update(unsigned long now);
};

#endif
