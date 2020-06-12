// Property management
//
// Copyright (c) 2020 Dave Astels

#include <Arduino.h>
#include "properties.h"
#include "logging.h"

extern Logger *logger;
extern const char *level_names[];
extern LogLevel level_for(const char *level_name);


uint16_t read_property_bits()
{
  uint16_t prop_bits = 0x0000;
  for (int pin = 29; pin <53; pin += 2) {
    pinMode(pin, INPUT_PULLDOWN);
    prop_bits <<= 1;
    if (digitalRead(pin)) {
      prop_bits |= 0x0001;
    }
  }
  return prop_bits;
}


Properties::Properties(uint16_t bits)
  : _fake_motors((bits & 0x0001) == 0x0001)
  , _log_level(level_for(level_names[(bits & 0x000E) >> 1]))
  , _log_to_serial((bits & 0x0010) == 0x0010)
{
}


Properties::Properties()
{
#if defined MOTORS
  _log_level = (LogLevel)(LOG);
  _fake_motors = (MOTORS) == 0;
  _log_to_serial = (LOG_TO_SERIAL) == 1;
#else
  Properties(read_property_bits());
#endif
}


void Properties::log()
{
  logger->info("Property settings:");
  logger->info("  Fake Motors: %s", _fake_motors ? "yes" : "no");
  logger->info("    Log level: %s", level_name_for(_log_level));
}
