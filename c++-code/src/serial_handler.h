// Serial logging handler (mainly for debug mode)
//
// Copyright (c) 2020 Dave Astels

#ifndef __SERIAL_HANDLER_H__
#define __SERIAL_HANDLER_H__

#include "logging_handler.h"

class SerialHandler: public LoggingHandler
{
 public:
  SerialHandler();
  void emit(const char *level_name, const char *msg);
};


#endif
