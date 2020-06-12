// Hardware interface
//
// Copyright (c) 2020 Dave Astels

#include "system.h"
#include "drive.h"
#include "hotspot_debouncable.h"
#include "in_your_face_debouncable.h"
#include "logging.h"

extern Logger *logger;


System::System(Properties *props)
  : _indicator(new Indicator())
  , _drive(Drive::make_drive(props->fake_motors(), this))
  , _propwing_power(new DigitalOutput(10))
  , _right_whisker(new Debouncer(new DigitalInput(0)))
  , _left_whisker(new Debouncer(new DigitalInput(1)))
  , _low_voltage(new Debouncer(new DigitalInput(4)))
  , _propwing_switch(new Debouncer(new DigitalInput(9)))
  , _ir_update_time(0)
  , _ir_update_interval(1000)
  , _accel(new Adafruit_LIS3DH())
  , _ir(new Ir())
  , _hotspot(new Debouncer(new HotspotDebouncable(_ir)))
  , _in_your_face(new Debouncer(new InYourFaceDebouncable(_ir)))
{
  logger->info("Starting LIS3DH...");
  if (!_accel->begin(0x18)) {   // change this to 0x19 for alternative i2c address
    logger->critical("Couldnt start LIS3DH ... stalling");
    while (1);
  }
  logger->info("LIS3DH Started!");
}


bool System::update(uint32_t now)
{
  _left_whisker->update();
  _right_whisker->update();
  _low_voltage->update();
  _propwing_switch->update();
  if (now >= _ir_update_time ) {
    _ir_update_time = now + _ir_update_interval;
    _ir->update(now);
    _hotspot->update();
    _in_your_face->update();
    return true;
  }
  return false;
}


void System::power_off()
{
  _drive->disable();
  _propwing_power->clear();
}


void System::power_on()
{
  _drive->enable();
  _propwing_power->set();
  _indicator->begin();
}
