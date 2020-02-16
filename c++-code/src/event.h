// -*- mode: c++ -*-
// Event class
//
// Copyright (c) 2020 Dave Astels

#ifndef __EVENT_H__
#define __EVENT_H__

enum class EventSubsystem {HEARTBEAT, WHISKER, IR};
enum class EventType {HOTSPOT, INYOURFACE, FOCUS, LEFT, RIGHT};
enum class EventFocus {NONE, LEFT, CENTER, RIGHT};

struct Event {
  EventSubsystem subsystem;
  EventType type;
  union {
    bool bool_value;
    int int_value;
    EventFocus focus_value;
    char *str_value;
  };
};

#endif
