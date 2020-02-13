// -*- mode: c++ -*-
// Behaviour management class
//
// Copyright (c) 2020 Dave Astels

#ifndef __BEHAVIOURS_H__
#define __BEHAVIOURS_H__


#include "behaviour.h"
#include "event.h"

class System;

#define MAX_BEHAVIOURS (32)

class Behaviours
{
  // lowest level behaviour is at index 0
  Behaviour *_behaviours[MAX_BEHAVIOURS];
  uint8_t _number_of_behaviours;
  System *_system;

 public:
  Behaviours(System *sys);
  bool add(Behaviour *behaviour);
  void start();
  void update(uint32_t now);
  void event_occurred(Event *event);
  inline bool empty() { return _number_of_behaviours == 0; }


};

#endif
