// Debouncable class for detecting a full battery
//
// Copyright (c) 2022 Dave Astels

#include "full_battery_debouncable.h"

FullBatteryDebouncable::FullBatteryDebouncable(FuelGauge *fuel_gauge)
  : _fuel_gauge(fuel_gauge)
{
}

bool FullBatteryDebouncable::value()
{
  return _fuel_gauge->percentage() >= 95;
}
