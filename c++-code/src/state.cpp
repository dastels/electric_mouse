// State base class
//
// Copyright (c) 2020 Dave Astels

#include "state.h"
#include "state_machine.h"


State::State(StateMachine &machine, String16 &name)
  : _machine(&machine)
  , _name(name)
{
}

void State::go_to(String16 &state_name, void *data)
{
  _machine->go_to_state(state_name, data);
}
