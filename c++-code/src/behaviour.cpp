// Base behaviour class
//
// Copyright (c) 2020 Dave Astels

#include "behaviour.h"

Behaviour::Behaviour(System &system, String16 &name)
  : StateMachine(system, name)
  , _status(INACTIVE)
  , _previous_status(NONE)
{
}


void Behaviour::subsume(Behaviour *b)
{
  _subsumed_behaviour = b;
}


void Behaviour::activate()
{
  if (_subsumed_behaviour) {
    _subsumed_behaviour->surpress();
  }
  _status = ACTIVE;
  reset();
}


void Behaviour::deactivate()
{
  if (_subsumed_behaviour) {
    _subsumed_behaviour->unsurpress();
  }
  _status = INACTIVE;
  reset();
}


void Behaviour::surpress()
{
  _previous_status = _status;
  _status = SURPRESSED;
  if (_subsumed_behaviour) {
    _subsumed_behaviour->surpress();
  }
}


void Behaviour::unsurpress()
{
  _status = _previous_status;
  if (_subsumed_behaviour && _status != ACTIVE) {
    _subsumed_behaviour->unsurpress();
  }
}


bool Behaviour::active()
{
  return _status == ACTIVE;
}


void Behaviour::update(uint32_t now)
{
  if (active()) {
    StateMachine::update(now);
  }
}
