// State Machine base class
//
// Copyright (c) 2020 Dave Astels

#include "state_machine.h"

StateMachine::StateMachine(System &system, String16 &name)
  : _system(&system)
  , _name(name)
{
}


StateMachine::~StateMachine()
{
  _states.clear();
}


bool StateMachine::add_state(State &state)
{
  if (_states.full()) return false;
  _states.insert(etl::pair<String16, State>(state.name(), state));
  return true;
}


void StateMachine::reset()
{
  go_to_state(_initial_state_name);
}


bool StateMachine::go_to_state(String16 &state_name, void *data)
{
  if (state_name.empty()) return false;
  try {
    State &new_state = _states.at(state_name);
    _current_state->exit();
    _current_state = &new_state;
    _current_state->enter(data);
    return true;
  } catch (etl::map_out_of_bounds) {
    return false;
  }
}


void StateMachine::update(uint32_t now)
{
  if (_current_state) {
    _current_state->update(now);
  }
}


void StateMachine::event_occurred(Event &event)
{
  if (_current_state) {
    _current_state->event_occurred(event, *this);
  }
}


String16 StateMachine::current_state_name()
{
  if (_current_state) {
    return _current_state->name();
  } else {
    return "NOT_SET";
  }
}


String16 StateMachine::name()
{
  return _name;
}


System *StateMachine::system()
{
  return _system;
}
