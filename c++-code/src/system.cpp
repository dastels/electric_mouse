// Hardware interface
//
// Copyright (c) 2020 Dave Astels

#include "system.h"
#include "drive.h"
#include "hotspot_debouncable.h"
#include "in_your_face_debouncable.h"


System::System(bool debug_mode)
  : _strip(new Adafruit_NeoPixel(1, 5, NEO_GRB + NEO_KHZ800))
  , _drive(Drive::make_drive(debug_mode, this))
  , _propwing_power(new DigitalOutput(10))
  , _right_whisker(new Debouncer(new DigitalInput(0)))
  , _left_whisker(new Debouncer(new DigitalInput(1)))
  , _low_voltage(new Debouncer(new DigitalInput(4)))
  , _propwing_switch(new Debouncer(new DigitalInput(9)))
  , _ir_update_time(0)
  // , _ir(new Ir())
  // , _hotspot(new Debouncer(new HotspotDebouncable(_ir)))
  // , _in_your_face(new Debouncer(new InYourFaceDebouncable(_ir)))
{
}


void System::update(uint32_t now)
{
  _left_whisker->update();
  _right_whisker->update();
  _low_voltage->update();
  _propwing_switch->update();
  // if (now >= _ir_update_time ) {
  //   _ir_update_time = now + IR_UPDATE_INTERVAL;
  //   _ir->update(now);
  // }
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
  _strip->begin();
  _strip->show();
}


void System::indicate(uint8_t red, uint8_t green, uint8_t blue)
{
  _strip->setPixelColor(0, red, green, blue);
  _strip->show();
}
