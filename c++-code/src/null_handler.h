// Serial logging handler (mainly for debug mode)
//
// Copyright (c) 2020 Dave Astels

#ifndef __NULL_HANDLER_H__
#define __NULL_HANDLER_H__

#include "logging_handler.h"

class NullHandler: public LoggingHandler
{
 public:
  NullHandler();
  void emit(const char *level_name, const char *msg);
};


#endif
