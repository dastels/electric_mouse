// -*- mode: c++ -*-
// Hardware interface
//
// Copyright (c) 2020 Dave Astels

#ifndef __IR_H__
#define __IR_H__

#include <Adafruit_MLX90640.h>
#include "event.h"

enum class IrSlice {ALL, LEFT, CENTER, RIGHT};

const char *name_of(IrSlice slice);

class Ir
{
  float _frame[768];
  int _left_width;
  int _right_width;
  int _center_size_scaling_factor;
  Adafruit_MLX90640 *_sensor;
  bool _good_frame;
  int _update_log_count;
  float _threshold;

  int start_of(IrSlice slice);
  int end_of(IrSlice slice);
  int width_of(IrSlice slice);

  void log_if_time();
  int max_of_three(int v1, int v2, int v3);
  float compute_average(IrSlice slice);
  float compute_hottest(IrSlice slice);
  float compute_coldest(IrSlice slice);
  int compute_percent_above(IrSlice slice);
public:
  Ir();
  void threshold(float th);
  int pixels_in(IrSlice slice);
  float average(IrSlice slice);
  float hottest(IrSlice slice);
  float coldest(IrSlice slice);
  int percent_above(IrSlice slice);
  void split(int left_width, int right_width);
  EventFocus focus(int percentage_threshold);
  void update();
};

#endif
