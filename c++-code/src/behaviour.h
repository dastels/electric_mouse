// -*- mode: c++ -*-
// Base behaviour class
//
// Copyright (c) 2020 Dave Astels

#ifndef __BEHAVIOUR_H__
#define __BEHAVIOUR_H__

#include "state_machine.h"

enum class  BehaviourStatus {NONE, INACTIVE, ACTIVE, SURPRESSED};

class Behaviour: public StateMachine
{
  Behaviour *_subsumed_behaviour;
  BehaviourStatus _status;
  BehaviourStatus _previous_status;

 public:
  Behaviour(System *system, const char *name);
  void subsume(Behaviour *b);
  void activate();
  void deactivate();
  void surpress();
  void unsurpress();
  bool active();
  void update(uint32_t now);
};

#endif
