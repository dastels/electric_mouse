// -*- mode: c++ -*-
// TOF distance sensor interface
//
// Copyright (c) 2022 Dave Astels

#include "range.h"
#include "event.h"

TofDirection directions[8] = {TofDirection::AHEAD_LEFT,
                              TofDirection::AHEAD,
                              TofDirection::AHEAD_RIGHT,
                              TofDirection::RIGHT,
                              TofDirection::BEHIND_RIGHT,
                              TofDirection::BEHIND,
                              TofDirection::BEHIND_LEFT,
                              TofDirection::LEFT};


Range::Range()
{
  for (uint8_t channel = 0; channel < 8; channel++) {
    _tofs[channel] = new Tof(directions[channel], channel);
  }
}


void Range::register_callback(RangeCallback f, uint8_t threshold)
{
  _callback = f;
  _callback_threshold = threshold;
}


void Range::update()
{
  for (uint8_t channel = 0; channel < 8; channel++) {
    _tofs[channel]->update();
  }
  if (_callback_threshold > 0) { // callback active
    Tof *closest_tof = closest();
    if (closest_tof->range() < _callback_threshold) {
      (*_callback)(closest_tof);
    }
  }
}


Tof *Range::closest()
{
  Tof *tof = _tofs[0];
  for (uint8_t channel = 1; channel < 8; channel++) {
    if (_tofs[channel]->range() < tof->range()) {
      tof = _tofs[channel];
    }
  }
  return tof;
}


uint8_t Range::range(TofDirection dir) {
  return _tofs[static_cast<int>(dir)]->range();
}


float Range::average_lux()
{
  float avg = 0.0;
  for (uint8_t channel = 1; channel < 8; channel++) {
    avg += _tofs[channel]->lux();
  }
  return avg / 8;
}


float Range::lux(TofDirection dir) {
  return _tofs[static_cast<int>(dir)]->lux();
}
