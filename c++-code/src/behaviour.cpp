// Base behaviour class
//
// Copyright (c) 2020 Dave Astels

#include "logging.h"
#include "behaviour.h"


extern Logger *logger;


Behaviour::Behaviour(System *system, const char *name, bool should_log_transitions)
  : StateMachine(system, name, should_log_transitions)
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
  logger->debug_mid("Activating: %s", _name);
  if (_subsumed_behaviour) {
    _subsumed_behaviour->surpress();
  }
  _status = BehaviourStatus::ACTIVE;
  reset();
}


void Behaviour::deactivate()
{
  logger->debug_mid("Deactivating: %s", _name);
  if (_subsumed_behaviour) {
    _subsumed_behaviour->unsurpress();
  }
  _status = BehaviourStatus::INACTIVE;
  reset();
}


void Behaviour::surpress()
{
  if (_status != BehaviourStatus::SURPRESSED) {
    logger->debug_deep("Surpressing: %s", _name);
    _previous_status = _status;
    _status = BehaviourStatus::SURPRESSED;
    reset();
    if (_subsumed_behaviour) {
      _subsumed_behaviour->surpress();
    }
  }
}


void Behaviour::unsurpress()
{
  if (_status == BehaviourStatus::SURPRESSED) {
    logger->debug_deep("Unsurpressing: %s", _name);
    _status = _previous_status;
    if (_subsumed_behaviour && _status != BehaviourStatus::ACTIVE) {
      _subsumed_behaviour->unsurpress();
    }
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
