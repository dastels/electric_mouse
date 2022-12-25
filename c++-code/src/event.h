// -*- mode: c++ -*-
// Event class
//
// Copyright (c) 2020 Dave Astels

#ifndef __EVENT_H__
#define __EVENT_H__

enum class EventSubsystem {HEARTBEAT, RANGE, LUX, IR, BATTERY};
enum class EventType {VALUE, HOTSPOT, INYOURFACE, FOCUS, CLOSE, LOW, FULL};
enum class EventFocus {NONE, LEFT, CENTER, RIGHT};

const char *name_of_focus(EventFocus focus);

class Event {
public:
  EventSubsystem subsystem;
  EventType type;
  union {
    bool bool_value;
    int int_value;
    EventFocus focus_value;
    char *str_value;
  };

  Event(EventSubsystem subsystem);
  Event(EventSubsystem subsystem, EventType type);
  Event(EventSubsystem subsystem, EventType type, bool value);
  Event(EventSubsystem subsystem, EventType type, int value);
  Event(EventSubsystem subsystem, EventType type, EventFocus value);
  Event(EventSubsystem subsystem, EventType type, char * value);

  const char *subsystem_str();
  const char *type_str();
  const char *value_str();
  const char *to_string();
};

#endif
