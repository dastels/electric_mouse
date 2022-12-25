// -*- mode: c++ -*-
// Fuel gauge interface
//
// Copyright (c) 2022 Dave Astels

#ifndef __FUEL_GAUGE_H__
#define __FUEL_GAUGE_H__

#include <Adafruit_LC709203F.h>

class FuelGauge
{
  Adafruit_LC709203F *_lc;
  float _cached_voltage;
  float _cached_percentage;
public:
  FuelGauge();
  void update();
  float percentage() { return _cached_percentage; }
  float voltage() { return _cached_voltage; }
};

#endif __FUELGAUGE_H__
