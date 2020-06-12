// -*- mode: c++ -*-
// Go-Forward behaviour
//
// Copyright (c) 2020 Dave Astels

#ifndef __BEHAVIOUR_WHISKER_H__
#define __BEHAVIOUR_WHISKER_H__

#include <stdint.h>

#include "behaviour.h"
#include "system.h"


class BehaviourWhisker: public Behaviour
{
 public:
  BehaviourWhisker(System *system, bool should_log_transitions);
};


#endif
