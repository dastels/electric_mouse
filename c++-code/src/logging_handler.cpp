// Abstract logging handler
//
// Copyright (c) 2020 Dave Astels

#include <Arduino.h>
#include "logging_handler.h"


LoggingHandler::LoggingHandler()
{
}

char *LoggingHandler::format(const char *level_name, const char *msg)
{
  sprintf(_buffer, "%lu - %s: %s", millis(), level_name, msg);
  return _buffer;
}
