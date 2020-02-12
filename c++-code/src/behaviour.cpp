// Base behaviour class
//
// Copyright (c) 2020 Dave Astels

#include "behaviour.h"

Behaviour::Behaviour(System *system, const char *name)
  : StateMachine(system, name)
  , _status(BehaviourStatus::INACTIVE)
  , _previous_status(BehaviourStatus::NONE)
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
  _status = BehaviourStatus::ACTIVE;
  reset();
}


void Behaviour::deactivate()
{
  if (_subsumed_behaviour) {
    _subsumed_behaviour->unsurpress();
  }
  _status = BehaviourStatus::INACTIVE;
  reset();
}


void Behaviour::surpress()
{
  _previous_status = _status;
  _status = BehaviourStatus::SURPRESSED;
  if (_subsumed_behaviour) {
    _subsumed_behaviour->surpress();
  }
}


void Behaviour::unsurpress()
{
  _status = _previous_status;
  if (_subsumed_behaviour && _status != BehaviourStatus::ACTIVE) {
    _subsumed_behaviour->unsurpress();
  }
}


bool Behaviour::active()
{
  return _status == BehaviourStatus::ACTIVE;
}


void Behaviour::update(uint32_t now)
{
  if (active()) {
    StateMachine::update(now);
  }
}
