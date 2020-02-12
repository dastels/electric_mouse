// -*- mode: c++ -*-
// State Machine base class
//
// Copyright (c) 2020 Dave Astels

#ifndef __STATE_MACHINE_H__
#define __STATE_MACHINE_H__

#include "system.h"
#include "state.h"
#include "event.h"
#include "util.h"

#define MAX_STATES (16)

class StateMachine {
 protected:
  System *_system;
  const char *_initial_state_name;

 private:
  const char *_name;
  State *_current_state;
  const char *_state_names[MAX_STATES];
  State *_states[MAX_STATES];
  int _number_of_states;

  State *find_state(const char *state_name);

 public:
  StateMachine(System *system, const char *name);
  bool add_state(State *state);
  void reset();
  bool go_to_state(const char *state_name, void *data=nullptr);
  virtual void update(uint32_t now);
  virtual void event_occurred(Event &event);
  const char *current_state_name();
  const char *name() { return _name; }
  System *system() { return _system; }
};

#endif
