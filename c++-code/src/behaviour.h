// Base behaviour class
//
// Copyright (c) 2020 Dave Astels

#ifndef __BEHAVIOUR_H__
#define __BEHAVIOUR_H__

#include "state_machine.h"

typedef enum {NONE, INACTIVE, ACTIVE, SURPRESSED} BehaviourStatus;

class Behaviour: public StateMachine
{
  Behaviour *_subsumed_behaviour;
  BehaviourStatus _status;
  BehaviourStatus _previous_status;

 public:
  Behaviour(System &system, String16 &name);
  void subsume(Behaviour *b);
  void activate();
  void deactivate();
  void surpress();
  void unsurpress();
  bool active();
  virtual void update(uint32_t now);
};

#endif
