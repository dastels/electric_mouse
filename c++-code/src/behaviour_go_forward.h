// Go-Forward behaviour
//
// Copyright (c) 2020 Dave Astels

#ifndef __BEHAVIOUR_GO_FORWARD_H__
#define __BEHAVIOUR_GO_FORWARD_H__

#include <stdint.h>

#include "state.h"
#include "state_machine.h"
#include "behaviour.h"

class GoForwardState: public State
{
  Drive *_drive;

 public:
  GoForwardState(StateMachine *owner_machine);
  virtual void update(uint32_t now);
};


class BehaviourGoForward: public Behaviour
{
 public:
  BehaviourGoForward(System *system);
};


#endif
