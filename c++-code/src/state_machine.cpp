// State Machine base class
//
// Copyright (c) 2020 Dave Astels

#include <string.h>
#include "logging.h"
#include "state_machine.h"

extern Logger *logger;

StateMachine::StateMachine(System *system, const char *name, bool should_log_transitions)
  : _system(system)
  , _name(name)
  , _current_state(NULL)
  , _number_of_states(0)
  , _log_transitions(should_log_transitions)
{
}


bool StateMachine::add_state(State *state)
{
  if (_number_of_states == MAX_STATES) {
    logger->critical("Adding state %s to %s failed", state->name(), _name);
    return false;
  }
  _states[_number_of_states++] = state;
  return true;
}


bool StateMachine::reset()
{
  if (go_to_state(_initial_state_name)) {
    return true;
  }
  logger->critical("Resetting %s to %s failed!", _name, _initial_state_name);
  return false;
}


State *StateMachine::find_state(const char *state_name)
{
  for (int index = 0; index < _number_of_states; index++) {
    if (_states[index]->is_named(state_name)) {
      return _states[index];
    }
  }
  return nullptr;
}


bool StateMachine::go_to_state(const char *state_name, void *data)
{
  if (!state_name || !*state_name) { // no state name
    logger->critical("No state name to go to in %s", _name);
    return false;
  }
  State *new_state = find_state(state_name);
  if (new_state == nullptr) {   // bad state name
    logger->critical("Tried to go to bad state %s in %s", state_name, _name);
    return false;
  }
  if (_current_state) {
    if (_log_transitions) logger->debug("Exiting %s:%s", name(), _current_state->name());
    _current_state->exit();
  }
  _current_state = new_state;
  if (_log_transitions) logger->debug("Entering %s:%s", name(), _current_state->name());
  _current_state->enter(data);
  return true;
}


void StateMachine::update(uint32_t now)
{
  if (_current_state) {
    _current_state->update(now);
  }
}


void StateMachine::event_occurred(Event *event)
{
  if (_current_state) {
    _current_state->event_occurred(event);
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
