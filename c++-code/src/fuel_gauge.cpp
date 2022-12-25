// -*- mode: c++ -*-
// Fuel gauge interface
//
// Copyright (c) 2022 Dave Astels

#include "fuel_gauge.h"

FuelGauge::FuelGauge()
  :_lc(new Adafruit_LC709203F())
  ,_cached_voltage(0)
  ,_cached_percentage(0)
{
  _lc->begin(&Wire1);
}

void FuelGauge::update()
{
  _cached_voltage = _lc->cellVoltage();
  _cached_percentage = _lc->cellPercent();
}
