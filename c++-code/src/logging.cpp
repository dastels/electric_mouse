// Main mouse code
//
// Copyright (c) 2020 Dave Astels

#include <Arduino.h>
#include <stdarg.h>
#include <stdio.h>
#include "logging.h"

const char *level_names[] = {"NOTSET", "DEBUG", "INFO", "WARNING", "ERROR", "CRITICAL"};

const char *level_for(int level)
{
  if (level == NOTSET) {
    return "NOTSET";
  } else if (level < DEBUG) {
    return "DEBUG";
  } else if (level < INFO) {
    return "INFO";
  } else if (level < WARNING) {
    return "WARNING";
  } else if (level < ERROR) {
    return "ERROR";
  } else if (level < CRITICAL) {
    return "CRITICAL";
  } else {
    return "> CRITICAL";
  }
}


LoggingHandler::LoggingHandler()
{
}

char *LoggingHandler::format(int level, const char *msg)
{
  sprintf(_buffer, "%lu - %s(%d): %s", millis(), level_for(level), level, msg);
  return _buffer;
}


SerialHandler::SerialHandler(): LoggingHandler()
{
}


void SerialHandler::emit(int level, const char *msg)
{
  Serial.println(format(level, msg));
}


Logger *Logger::_instance = nullptr;


Logger *Logger::get_logger()
{
  if (_instance == nullptr) {
    _instance = new Logger();
  }
  return _instance;
}


Logger::Logger(): _handler(new SerialHandler())
{
}


void Logger::_internal_log(int level, const char *format, va_list args)
{
  if (level >= _level) {
    vsprintf(_buffer, format, args);
    _handler->emit(level, _buffer);
  }
}


void Logger::set_level(int new_level)
{
  _level = new_level;
}


void Logger::set_handler(LoggingHandler *_handler)
{
  // not supported initially
}


void Logger::log(int level, const char *format, ...)
{
  va_list args;
  va_start (args, format);
  _internal_log(level, format, args);
  va_end (args);
}


void Logger::debug(const char *format, ...)
{
  va_list args;
  va_start (args, format);
  _internal_log(DEBUG, format, args);
  va_end (args);
}


void Logger::info(const char *format, ...)
{
  va_list args;
  va_start (args, format);
  _internal_log(INFO, format, args);
  va_end (args);
}


void Logger::warning(const char *format, ...)
{
  va_list args;
  va_start (args, format);
  _internal_log(WARNING, format, args);
  va_end (args);
}


void Logger::error(const char *format, ...)
{
  va_list args;
  va_start (args, format);
  _internal_log(ERROR, format, args);
  va_end (args);
}


void Logger::critical(const char *format, ...)
{
  va_list args;
  va_start (args, format);
  _internal_log(CRITICAL, format, args);
  va_end (args);
}
