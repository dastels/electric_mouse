// Behaviour management class
//
// Copyright (c) 2020 Dave Astels

#include <etl/vector.h>
#include <iterator>

#include "behaviours.h"

Behaviours::Behaviours()
  : _already_started(false)
{
}

void Behaviours::add(Behaviour &behaviour)
{
  Behaviour *subsumed = NULL;
  if (!_behaviours.empty()) {
    subsumed = &_behaviours[0];
  }
  behaviour.subsume(subsumed);
  _behaviours.insert(0, behaviour);
}


void Behaviours::start()
{
  if (_already_started) return;
  if (_behaviours.empty()) return;
  BehaviourStore::iterator last = _behaviours.end();
  last--;
  last->activate();
  _already_started = true;
}


void Behaviours::update(uint32_t now)
{
  start();
  BehaviourStore::iterator last = _behaviours.end();
  for (BehaviourStore::iterator it = _behaviours.begin(); it != last; it++) {
    it->update(now);
    if (it->active()) {
      return;
    }
  }
}


void Behaviours::event_occurred(Event &event)
{
  start();
  BehaviourStore::iterator last = _behaviours.end();
  for (BehaviourStore::iterator it = _behaviours.begin(); it != last; it++) {
    it->event_occurred(event);
    if (it->active()) {
      return;
    }
  }

}
