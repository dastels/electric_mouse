// Behaviour management class
//
// Copyright (c) 2020 Dave Astels

#ifndef __BEHAVIOURS_H__
#define __BEHAVIOURS_H__

#include <etl/vector.h>

#include "behaviour.h"
#include "event.h"

typedef etl::vector<Behaviour, 16> BehaviourStore;

class Behaviours
{
  BehaviourStore _behaviours;
  bool _already_started;

 public:
  Behaviours();
  void add(Behaviour &behaviour);
  void start();
  void update(uint32_t now);
  void event_occurred(Event &event);
};

#endif
