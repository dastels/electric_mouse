// IR sensor interface
//
// Copyright (c) 2020 Dave Astels

#include "ir.h"

Ir::Ir(float threshold)
  : _left_width(10)
  , _right_width(10)
  , _sensor(new Adafruit_MLX90640())
  , _threshold(threshold)
{
  _sensor->setMode(MLX90640_CHESS);
  _sensor->setResolution(MLX90640_ADC_18BIT);
  _sensor->setRefreshRate(MLX90640_8_HZ);
}


void Ir::threshold(float th)
{
  _threshold = th;
}


int Ir::pixels_in(IrSlice slice)
{
  return width_of(slice) * 24;
}


int Ir::start_of(IrSlice slice)
{
  switch (slice) {
  case IrSlice::LEFT:
    return 0;
    break;
  case IrSlice::CENTER:
    return _left_width;
    break;
  case IrSlice::RIGHT:
    return 32 - _right_width;
    break;
  case IrSlice::ALL:
    return 0;
    break;
  default:
    return -1;
    break;
  }
}


int Ir::end_of(IrSlice slice)
{
  switch (slice) {
  case IrSlice::LEFT:
    return _left_width;
    break;
  case IrSlice::CENTER:
    return 32 - _right_width;
    break;
  case IrSlice::RIGHT:
    return 32;
    break;
  case IrSlice::ALL:
    return 32;
    break;
  default:
    return -1;
    break;
  }
}


int Ir::width_of(IrSlice slice)
{
  switch (slice) {
  case IrSlice::LEFT:
    return _left_width;
    break;
  case IrSlice::CENTER:
    return 32 - (_right_width + _left_width);
    break;
  case IrSlice::RIGHT:
    return _right_width;
    break;
  case IrSlice::ALL:
    return 32;
    break;
  default:
    return -1;
    break;
  }
}


float Ir::average(IrSlice slice)
{
  int start = start_of(slice);
  int end = end_of(slice);
  float sum = 0;
  for (int y = 0; y < 24; y++) {
    for (int x = start; x < end; x++) {
      sum += _frame[y * 32 + x];
    }
  }
  return sum / (width_of(slice) * 24);
}


float Ir::hottest(IrSlice slice)
{
  int start = start_of(slice);
  int end = end_of(slice);
  float hot = 0;
  for (int y = 0; y < 24; y++) {
    for (int x = start; x < end; x++) {
      if (_frame[y * 32 + x] > hot) {
        hot = _frame[y * 32 + x];
      }
    }
  }
  return hot;
}


float Ir::coldest(IrSlice slice)
{
  int start = start_of(slice);
  int end = end_of(slice);
  float cold = 0;
  for (int y = 0; y < 24; y++) {
    for (int x = start; x < end; x++) {
      if (_frame[y * 32 + x] < cold) {
        cold = _frame[y * 32 + x];
      }
    }
  }
  return cold;
}


int Ir::percent_above(IrSlice slice)
{
  int start = start_of(slice);
  int end = end_of(slice);
  float above = 0;
  for (int y = 0; y < 24; y++) {
    for (int x = start; x < end; x++) {
      if (_frame[y * 32 + x] > _threshold) {
        above++;
      }
    }
  }
  return above;
}


void Ir::split(int left_width, int right_width)
{
  _left_width = left_width;
  _right_width = right_width;
}


EventFocus Ir::focus()
{
  int left_percent = percent_above(IrSlice::LEFT);
  int center_percent = percent_above(IrSlice::CENTER);
  int right_percent = percent_above(IrSlice::RIGHT);
  if (left_percent > 10 && left_percent > center_percent && left_percent > right_percent) {
    return EventFocus::LEFT;
  } else if (center_percent > 10 && center_percent > left_percent && center_percent > right_percent) {
    return EventFocus::CENTER;
  } else if (right_percent > 10 && right_percent > center_percent && right_percent > left_percent) {
    return EventFocus::RIGHT;
  } else {
    return EventFocus::NONE;
  }
}


void Ir::update(unsigned long now)
{
  _sensor->getFrame(_frame);
}
