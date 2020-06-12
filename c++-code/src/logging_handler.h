// Abstract logging handler
//
// Copyright (c) 2020 Dave Astels

#ifndef __LOGGING_HANDLER_H__
#define __LOGGING_HANDLER_H__


class LoggingHandler
{
  char _buffer[256];
 public:
  LoggingHandler();
  char *format(const char *level_name, const char *msg);
  virtual void emit(const char *level_name, const char *msg) = 0;
  virtual bool initialized() { return true; }
};


#endif
