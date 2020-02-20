// -*- mode: c++ -*-
// Logger
//
// Copyright (c) 2020 Dave Astels

#ifndef __LOGGER_H__
#define __LOGGER_H__

#define NOTSET   (0)
#define DEBUG    (10)
#define INFO     (20)
#define WARNING  (30)
#define ERROR    (40)
#define CRITICAL (50)

#include <stdarg.h>

class LoggingHandler
{
  char _buffer[256];
 public:
  LoggingHandler();
  char *format(int level, const char *msg);
  virtual void emit(int level, const char *msg) = 0;
};


class SerialHandler: public LoggingHandler
{
 public:
  SerialHandler();
  void emit(int level, const char *msg);
};


class Logger
{
  static Logger *_instance;
  LoggingHandler *_handler;
  int _level;
  char _buffer[256];
  void _internal_log(int level, const char *format, va_list args);

 public:
  Logger();
  static Logger *get_logger();
  void set_level(int new_level);
  void set_handler(LoggingHandler *_handler);
  void log(int level, const char *format, ...);
  void debug(const char *format, ...);
  void info(const char *format, ...);
  void warning(const char *format, ...);
  void error(const char *format, ...);
  void critical(const char *format, ...);
};


#endif
