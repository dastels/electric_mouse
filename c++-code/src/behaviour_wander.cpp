// Go-Forward behaviour
//
// Copyright (c) 2020 Dave Astels

#include "behaviour_wander.h"
#include "drive.h"

// local states

class IdleState: public State
{
  Drive *_drive;

 public:
  IdleState(StateMachine *owner_machine);
  void event_occurred(Event *event);
};


class LeftState: public State
{
  Drive *_drive;
  uint32_t _timeout;

public:
  LeftState(StateMachine *owner_machine);
  void enter(void *data);
  void exit();
  void update(uint32_t now);
};


class RightState: public State
{
  Drive *_drive;
  uint32_t _timeout;

 public:
  RightState(StateMachine *owner_machine);
  void enter(void *data);
  void exit();
  void update(uint32_t now);
};

// -----------------------------------------------------------------------------
// idle

IdleState::IdleState(StateMachine *owner_machine)
  : State(owner_machine, "idle")
  , _drive(owner_machine->system()->drive())
{
}


void IdleState::event_occurred(Event *event)
{
  if (strcmp(event->name(), "heartbeat") == 0) {
    int chance = random(100);
    if (chance < 50) {
      ((Behaviour*)_machine)->activate();
      if (chance < 25) {
        go_to("left");
      } else {
        go_to("right");
      }
    }

  }
}


// -----------------------------------------------------------------------------
//  left

LeftState::LeftState(StateMachine *owner_machine)
  : State(owner_machine, "left")
  , _drive(owner_machine->system()->drive())
  , _timeout(0)
{
}


void LeftState::enter(void *data)
{
  State::enter(data);
  _machine->system()->indicate(0x00, 0xFF, 0x00);
  _timeout = millis() + random(1, 25) * 20;
  switch (_drive->state()) {
  case DriveState::FORWARD:
    _drive->veer_left();
    break;
  case DriveState::STOPPED:
    _drive->pivot_left();
    break;
  default:
    break;
  }
}


void LeftState::exit()
{
  _machine->system()->indicate(0x00, 0x00, 0x00);
}


void LeftState::update(uint32_t now)
{
  if (now > _timeout) {
    ((Behaviour*)_machine)->deactivate();
  }
}


// -----------------------------------------------------------------------------
//  right

RightState::RightState(StateMachine *owner_machine)
  : State(owner_machine, "right")
  , _drive(owner_machine->system()->drive())
  , _timeout(0)
{
}


void RightState::enter(void *data)
{
  State::enter(data);
  _machine->system()->indicate(0x00, 0x00, 0xFF);
  _timeout = millis() + random(1, 25) * 20;
  switch (_drive->state()) {
  case DriveState::FORWARD:
    _drive->veer_right();
    break;
  case DriveState::STOPPED:
    _drive->pivot_right();
    break;
  default:
    break;
  }
}


void RightState::exit()
{
  _machine->system()->indicate(0x00, 0x00, 0x00);
}


void RightState::update(uint32_t now)
{
  if (now > _timeout) {
    ((Behaviour*)_machine)->deactivate();
  }
}


// -----------------------------------------------------------------------------
// wander behaviour

BehaviourWander::BehaviourWander(System *system)
  :Behaviour(system, "wander")
{
  add_state(new IdleState(this));
  add_state(new LeftState(this));
  add_state(new RightState(this));
  _initial_state_name = "idle";
  reset();
}
