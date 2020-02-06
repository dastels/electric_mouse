// State Machine base class
//
// Copyright (c) 2020 Dave Astels

#ifndef __STATE_MACHINE_H__
#define __STATE_MACHINE_H__

#include <etl/map.h>

#include "system.h"
#include "state.h"
#include "event.h"
#include "util.h"

class StateMachine {
  System *_system;
  String16 _name;
  State *_current_state;
  String16 _initial_state_name;
  etl::map<String16, State, 16> _states;

 public:
  StateMachine(System &system, String16 &name);
  ~StateMachine();
  bool add_state(State &state);
  void reset();
  bool go_to_state(String16 &state_name, void *data=nullptr);
  virtual void update(uint32_t now);
  virtual void event_occurred(Event &event);
  String16 current_state_name();
  String16 name();
  System *system();
};

#endif
