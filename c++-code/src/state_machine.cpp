// State Machine base class
//
// Copyright (c) 2020 Dave Astels

#include <string.h>
#include "state_machine.h"

StateMachine::StateMachine(System *system, const char *name)
  : _system(system)
  , _name(name)
  , _number_of_states(0)
{
}


bool StateMachine::add_state(State *state)
{
  if (_number_of_states == MAX_STATES) return false;
  _state_names[_number_of_states] = state->name();
  _states[_number_of_states++] = state;
  return true;
}


void StateMachine::reset()
{
  go_to_state(_initial_state_name);
}


State *StateMachine::find_state(const char *state_name)
{
  for (int index = 0; index < _number_of_states; index++) {
    if (strcmp(state_name, _state_names[index]) == 0) {
      return _states[index];
    }
  }
  return nullptr;
}


bool StateMachine::go_to_state(const char *state_name, void *data)
{
  if (!state_name || !*state_name) return false; // no state name
  State *new_state = find_state(state_name);
  if (new_state == nullptr) return false; // bad state name
  if (_current_state) {
    _current_state->exit();
  }
  _current_state = new_state;
  _current_state->enter(data);
  return true;
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


const char *StateMachine::current_state_name()
{
  if (_current_state) {
    return _current_state->name();
  } else {
    return "NOT_SET";
  }
}
