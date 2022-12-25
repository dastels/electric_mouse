// -*- mode: c++ -*-
// Hardware interface
//
// Copyright (c) 2020 Dave Astels

#ifndef __SYSTEM_H__
#define __SYSTEM_H__

#include "indicator.h"
#include "debouncer.h"
#include "digital_output.h"
#include "digital_input.h"
#include "properties.h"
#include "accel.h"
#include "ir.h"
#include "range.h"
#include "fuel_gauge.h"

class Drive;

class System
{
  Indicator *_indicator;
  Drive *_drive;

  Accel *_accel;
  unsigned long _accel_update_time;
  unsigned long _accel_update_interval;
  bool _accel_updated;

  Ir *_ir;
  unsigned long _ir_update_time;
  unsigned long _ir_update_interval;
  bool _ir_updated;
  Debouncer *_hotspot;
  Debouncer *_in_your_face;

  Range *_range;
  unsigned long _range_update_time;
  unsigned long _range_update_interval;
  bool _range_updated;

  FuelGauge *_fuel_gauge;
  unsigned long _fuel_gauge_update_time;
  unsigned long _fuel_gauge_update_interval;
  bool _fuel_gauge_updated;
  Debouncer *_low_battery;
  Debouncer *_full_battery;

 public:
  System(Properties *props);
  void set_ir_update_interval(unsigned long interval) { _ir_update_interval = interval; }
  void update(uint32_t now);
  bool accel_updated() { return _accel_updated; }
  bool fuel_gauge_updated() { return _fuel_gauge_updated; }
  bool ir_updated() { return _ir_updated; }
  bool range_updated() { return _range_updated; }
  Drive *drive() { return _drive; }
  Debouncer *low_battery() { return _low_battery; }
  Debouncer *full_battery() { return _full_battery; }
  Debouncer *hotspot() { return _hotspot; }
  Debouncer *in_your_face() { return _in_your_face; }
  Ir *ir() { return _ir; }
  Accel *accel() { return _accel; }
  Range *range() { return _range; }
  FuelGauge *fuel_gauge() { return _fuel_gauge; }
  Indicator *indicator() { return _indicator; }
  void power_off();
  void power_on();
};

#endif
