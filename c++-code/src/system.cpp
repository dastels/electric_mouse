// Hardware interface
//
// Copyright (c) 2020 Dave Astels

#include "config.h"
#include "system.h"
#include "drive.h"
#include "hotspot_debouncable.h"
#include "in_your_face_debouncable.h"
#include "low_battery_debouncable.h"
#include "full_battery_debouncable.h"
#include "logging.h"
#include "event.h"

extern Logger *logger;


System::System(Properties *props)
  : _indicator(new Indicator())
  , _drive(Drive::make_drive(props->fake_motors(), this))

  , _accel(new Accel())
  , _accel_update_time(0)
  , _accel_update_interval(DEFAULT_ACCEL_UPDATE_INTERVAL)
  , _accel_updated(false)

  , _ir(new Ir())
  , _ir_update_time(0)
  , _ir_update_interval(DEFAULT_IR_UPDATE_INTERVAL)
  , _ir_updated(false)

  , _range(new Range())
  , _range_update_time(0)
  , _range_update_interval(DEFAULT_RANGE_UPDATE_INTERVAL)
  , _range_updated(false)

  , _fuel_gauge(new FuelGauge())
  , _fuel_gauge_update_time(0)
  , _fuel_gauge_update_interval(DEFAULT_FUEL_GAUGE_UPDATE_INTERVAL)
  , _fuel_gauge_updated(false)

  , _hotspot(new Debouncer(new HotspotDebouncable(_ir)))
  , _in_your_face(new Debouncer(new InYourFaceDebouncable(_ir)))
  , _low_battery(new Debouncer(new LowBatteryDebouncable(_fuel_gauge)))
  , _full_battery(new Debouncer(new FullBatteryDebouncable(_fuel_gauge)))
{
}


void System::update(uint32_t now)
{
  if (now >= _ir_update_time) {
    _ir_update_time = now + _ir_update_interval;
    _ir->update();
    _hotspot->update();
    _in_your_face->update();
    _ir_updated = true;
      } else {
    _ir_updated = false;
  }
  if (now >= _range_update_time) {
    _range_update_time = now + _range_update_interval;
    _range->update();
    _range_updated = true;
  } else {
    _range_updated = false;
  }
  if (now >= _accel_update_time) {
    _accel_update_time = now + _accel_update_interval;
    _accel->update();
    _accel_updated = true;
  } else {
    _accel_updated = false;
  }
  if (now >= _fuel_gauge_update_time) {
    _fuel_gauge_update_time = now + _fuel_gauge_update_interval;
    _fuel_gauge->update();
    _low_battery->update();
    _full_battery->update();
    _fuel_gauge_updated = true;
  } else {
    _fuel_gauge_updated = false;
  }




}


void System::power_off()
{
}


void System::power_on()
{
}
