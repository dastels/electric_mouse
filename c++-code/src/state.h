// -*- mode: c++ -*-
// State base class
//
// Copyright (c) 2020 Dave Astels

#ifndef __STATE_H__
#define __STATE_H__

#include <stdint.h>

#include "util.h"
#include "event.h"

class StateMachine;

class State {
 protected:
  StateMachine *_machine;

 private:
  const char *_name;

 public:
  State(StateMachine *owner_machine=nullptr, const char *name=nullptr);
  const char *name() { return _name; }

  virtual void enter(void *data=nullptr) {}
  virtual void exit(void *data=nullptr) {}
  virtual void update(uint32_t now) {}
  virtual void event_occurred(Event *event) {}
  void go_to(const char *state_name, void *data=nullptr);
};

#endif
