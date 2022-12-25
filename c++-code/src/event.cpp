// Event class
//
// Copyright (c) 2020 Dave Astels

#include <stdio.h>
#include "logging.h"
#include "event.h"

extern Logger *logger;


const char *name_of_focus(EventFocus focus)
{
  switch (focus) {
  case EventFocus::NONE:
    return "NONE";
  case EventFocus::LEFT:
    return "LEFT";
  case EventFocus::CENTER:
    return "CENTER";
  case EventFocus::RIGHT:
    return "RIGHT";
  default:
    return "";
  }
}


Event::Event(EventSubsystem ss)
  : subsystem(ss)
{
}


Event::Event(EventSubsystem ss, EventType t)
  : subsystem(ss)
  , type(t)
{
}


Event::Event(EventSubsystem ss, EventType t, bool value)
  : subsystem(ss)
  , type(t)
  , bool_value(value)
{
}


Event::Event(EventSubsystem ss, EventType t, int value)
  : subsystem(ss)
  , type(t)
  , int_value(value)
{
}


Event::Event(EventSubsystem ss, EventType t, float value)
  : subsystem(ss)
  , type(t)
  , float_value(value)
{
}


Event::Event(EventSubsystem ss, EventType t, EventFocus value)
  : subsystem(ss)
  , type(t)
  , focus_value(value)
{
}


Event::Event(EventSubsystem ss, EventType t, char * value)
  : subsystem(ss)
  , type(t)
  , str_value(value)
{
}


const char *Event::subsystem_str()
{
  switch(subsystem) {
  case EventSubsystem::HEARTBEAT:
    return "HEARTBEAT";
  case EventSubsystem::RANGE:
    return "RANGE";
  case EventSubsystem::LUX:
    return "LUX";
  case EventSubsystem::IR:
    return "IR";
  }
  return "UNKNOWN_SUBSYSTEM";
}


const char *Event::type_str()
{
  if (subsystem == EventSubsystem::HEARTBEAT) {
    return "";
  }
  switch(type) {
  case EventType::VALUE:
    return "VALUE";
  case EventType::HOTSPOT:
    return "HOTSPOT";
  case EventType::INYOURFACE:
    return "INYOURFACE";
  case EventType::FOCUS:
    return "FOCUS";
  case EventType::CLOSE:
    return "CLOSE";
  }
  return "UNKNOWN_TYPE";
}


const char *Event::value_str()
{
  switch(subsystem) {
  case EventSubsystem::IR:
    switch(type) {
    case EventType::HOTSPOT:
    case EventType::INYOURFACE:
      return bool_value ? "TRUE" : "FALSE";
    case EventType::FOCUS:
      switch(focus_value) {
      case EventFocus::NONE:
        return "NONE";
      case EventFocus::LEFT:
        return "LEFT";
      case EventFocus::CENTER:
        return "CENTER";
      case EventFocus::RIGHT:
        return "RIGHT";
      default:
        return "";
      }
    default:
      return "";
    }
  case EventSubsystem::RANGE:
    if (type == EventType::CLOSE) {
      return ""; //int_value
    } else {
      return "No Value";
    }
  case EventSubsystem::LUX:
    if (type == EventType::VALUE) {
      return ""; //float_value
    } else {
      return "No Value";
    }
  default:
    return "";
  }
}

char str_buffer[32];

const char *Event::to_string()
{
  const char *vs = value_str();
  sprintf(str_buffer, "<Event %s:%s%s%s>", subsystem_str(), type_str(), *vs ? ":" : "", vs);
  return str_buffer;
}
