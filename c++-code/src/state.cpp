// State base class
//
// Copyright (c) 2020 Dave Astels

#include "state.h"
#include "state_machine.h"


State::State(StateMachine *machine, const char *name)
  : _machine(machine)
  , _name(name)
{
}


bool State::is_named(const char *name)
{
  return strcmp(name, _name) == 0;
}


void State::go_to(const char *state_name, void *data)
{
  _machine->go_to_state(state_name, data);
}
