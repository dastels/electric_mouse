// -*- mode: c++ -*-
// Debouncable class for detecting full battery
//
// Copyright (c) 2022 Dave Astels

#ifndef __FULL_BATTERY_DEBOUNCABLE_H__
#define __FULL_BATTERY_DEBOUNCABLE_H__

#include "fuel_gauge.h"
#include "debouncable.h"

class FullBatteryDebouncable: public Debouncable
{
  FuelGauge *_fuel_gauge;

public:
  FullBatteryDebouncable(FuelGauge *fuel_gauge);
  bool value();
};

#endif
