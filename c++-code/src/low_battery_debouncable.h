// -*- mode: c++ -*-
// Debouncable class for detecting low battery
//
// Copyright (c) 2022 Dave Astels

#ifndef __LOW_BATTERY_DEBOUNCABLE_H__
#define __LOW_BATTERY_DEBOUNCABLE_H__

#include "fuel_gauge.h"
#include "debouncable.h"

class LowBatteryDebouncable: public Debouncable
{
  FuelGauge *_fuel_gauge;

public:
  LowBatteryDebouncable(FuelGauge *fuel_gauge);
  bool value();
};

#endif
