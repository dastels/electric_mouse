// Behaviour management class
//
// Copyright (c) 2020 Dave Astels

#include <iterator>

#include "behaviours.h"
#include "system.h"

Behaviours::Behaviours(System *sys)
  : _number_of_behaviours(0)
  , _system(sys)
{
}

bool Behaviours::add(Behaviour *behaviour)
{
  Behaviour *subsumed = nullptr;
  if (_number_of_behaviours == MAX_BEHAVIOURS) return false;
  if (!empty()) {
    subsumed = _behaviours[_number_of_behaviours - 1];
  }
  behaviour->subsume(subsumed);
  _behaviours[_number_of_behaviours++] = behaviour;
  return true;
}


void Behaviours::start()
{
  if (empty()) return;
  _behaviours[0]->activate();
}


void Behaviours::update(uint32_t now)
{
  for (uint8_t index = _number_of_behaviours - 1; index >= 0; index--) {
    _behaviours[index]->update(now);
    if (_behaviours[index]->active()) {
      return;
    }
  }
}


void Behaviours::event_occurred(Event *event)
{
  for (uint8_t index = _number_of_behaviours - 1; index >= 0; index--) {
    _behaviours[index]->event_occurred(event);
    if (_behaviours[index]->active()) {
      return;
    }
  }

}
