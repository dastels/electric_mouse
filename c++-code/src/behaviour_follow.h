// -*- mode: c++ -*-
// Follow a hot body behaviour
//
// Copyright (c) 2020 Dave Astels

#ifndef __BEHAVIOUR_FOLLOW_H__
#define __BEHAVIOUR_FOLLOW_H__

#include <stdint.h>

#include "behaviour.h"

class BehaviourFollow: public Behaviour
{
 public:
  BehaviourFollow(System *system, bool should_log_transitions);
};


#endif
