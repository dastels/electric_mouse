// -*- mode: c++ -*-
// TOF distance sensor interface
//
// Copyright (c) 2022 Dave Astels

#ifndef __TOF_H__
#define __TOF_H__

#include <Adafruit_VL6180X.h>
#include "event.h"

#define TCAADDR 0x70

enum class TofDirection {AHEAD_LEFT, AHEAD, AHEAD_RIGHT, RIGHT, BEHIND_RIGHT, BEHIND, BEHIND_LEFT, LEFT};


class Tof
{
  TofDirection _direction;          /* The direction the TOF is facing */
  uint8_t _cached_range;                    /* range reading */
  float _cached_lux;
  Adafruit_VL6180X *_vl;
  uint8_t _channel;
  void report_status(uint8_t status);
  const char *direction_string();
  void tca_select(uint8_t i);
 public:
  Tof(TofDirection direction, int channel);
  TofDirection direction() { return _direction; }
  uint8_t channel() { return _channel; }
  uint8_t range() { return _cached_range; }
  float lux() { return _cached_lux; }
  void update();
};


#endif __TOF_H__
