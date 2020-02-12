// Go-Forward behaviour
//
// Copyright (c) 2020 Dave Astels

#include "behaviour_go_forward.h"
#include "drive.h"

GoForwardState::GoForwardState(StateMachine *owner_machine)
  : State(owner_machine, "GoForward")
  , _drive(owner_machine->system()->drive())
{
}


void GoForwardState::update(uint32_t now)
{
  _drive->forward();
}


BehaviourGoForward::BehaviourGoForward(System *system)
  :Behaviour(system, "Forward")
{
  add_state(new GoForwardState(this));
  _initial_state_name = "GoForward";
  reset();
}
