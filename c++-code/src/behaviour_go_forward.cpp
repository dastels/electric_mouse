// Go-Forward behaviour
//
// Copyright (c) 2020 Dave Astels

#include "behaviour_go_forward.h"
#include "drive.h"

GoForwardState::GoForwardState(StateMachine *owner_machine)
  : State(owner_machine, "go-forward")
  , _drive(owner_machine->system()->drive())
{
}


void GoForwardState::update(uint32_t now)
{
  _drive->forward();
}


BehaviourGoForward::BehaviourGoForward(System *system)
  :Behaviour(system, "forward")
{
  add_state(new GoForwardState(this));
  _initial_state_name = "go-forward";
  reset();
}
