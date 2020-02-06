// State base class
//
// Copyright (c) 2020 Dave Astels

#ifndef __STATE_H__
#define __STATE_H__

#include "util.h"
#include "event.h"

class StateMachine;

class State {
  StateMachine *_machine;
  String16 _name;

 public:
  State(StateMachine &owner_machine, String16 &name);
  String16 name();

  virtual void enter(void *data=nullptr) {}
  virtual void exit(void *data=nullptr) {}
  virtual void update(uint32_t now) {}
  virtual void event_occurred(Event &event, StateMachine &machine) {}
  void go_to(String16 &state_name, void *data=nullptr);
};

#endif
