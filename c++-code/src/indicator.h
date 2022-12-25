// -*- mode: c++ -*-
// Indicator strip interface
//
// Copyright (c) 2020 Dave Astels

#ifndef __INDICATOR_H__
#define __INDICATOR_H__

#include <Adafruit_NeoPixel.h>
#include "event.h"

class Indicator
{
  Adafruit_NeoPixel *_strip;
  void show_speed(int pixel, int speed);
 public:
  Indicator();
  void begin();
  void clear();
  void fill(uint8_t red, uint8_t green, uint8_t blue);
  void show();
  void setPixelColour(uint8_t pixel_number, uint8_t red, uint8_t green, uint8_t blue);

  void left_drive(int speed);
  void right_drive(int speed);
  void in_your_face(bool present);
  void hotspot(EventFocus focus);
};


#endif
