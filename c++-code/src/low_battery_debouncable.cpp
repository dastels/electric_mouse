// Debouncable class for detecting a low battery
//
// Copyright (c) 2022 Dave Astels

#include "low_battery_debouncable.h"

LowBatteryDebouncable::LowBatteryDebouncable(FuelGauge *fuel_gauge)
  : _fuel_gauge(fuel_gauge)
{
}

bool LowBatteryDebouncable::value()
{
  return _fuel_gauge->percentage() <= 10;
}
