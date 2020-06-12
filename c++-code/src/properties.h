// -*- mode: c++ -*-
// Properties records and gives access to various system settings
//
// Copyright (c) 2020 Dave Astels

#ifndef __PROPERTIES_H__
#define __PROPERTIES_H__

#include <stdint.h>
#include "logging.h"

// Sone constants to help

#define FAKE_MOTORS (1)
#define REAL_MOTORS (0)

#define LOG_LEVEL_TO_PROPERTY(level) ((int)(level) << 1)

class Properties {
  bool _fake_motors;            // true means use simulated motors with logged output
  LogLevel _log_level;          // the level to set the logger to
  bool _log_to_serial;          // whether to send logging to serial

 public:
  Properties();
  Properties(uint16_t bits);
  void log();
  bool fake_motors() { return _fake_motors; }
  bool log_to_serial() { return _log_to_serial; }
  LogLevel log_level() { return _log_level; }
};



#endif
