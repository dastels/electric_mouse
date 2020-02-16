// -*- mode: c++ -*-
// Go-Forward behaviour
//
// Copyright (c) 2020 Dave Astels

#ifndef __BEHAVIOUR_WANDER_H__
#define __BEHAVIOUR_WANDER_H__

#include <stdint.h>

#include "behaviour.h"
#include "system.h"


class BehaviourWander: public Behaviour
{
 public:
  BehaviourWander(System *system);
};


#endif
