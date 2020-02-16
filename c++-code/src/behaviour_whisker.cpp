// Go-Forward behaviour
//
// Copyright (c) 2020 Dave Astels

#include "behaviour_whisker.h"
#include "drive.h"

const unsigned long REVERSE_TIME = 250;

namespace {
  // local states

  class IdleState: public State
  {
  public:
    IdleState(StateMachine *owner_machine);
    void event_occurred(Event *event);
  };


  class ReverseState: public State
  {
    Drive *_drive;
    uint32_t _timeout;
    const char *_data;

  public:
    ReverseState(StateMachine *owner_machine);
    void enter(void *data);
    void update(uint32_t now);
  };


  class LeftState: public State
  {
    Drive *_drive;
    uint32_t _timeout;

  public:
    LeftState(StateMachine *owner_machine);
    void enter(void *data);
    void update(uint32_t now);
  };


  class RightState: public State
  {
    Drive *_drive;
    uint32_t _timeout;

  public:
    RightState(StateMachine *owner_machine);
    void enter(void *data);
    void update(uint32_t now);
  };
}

// -----------------------------------------------------------------------------
// idle

IdleState::IdleState(StateMachine *owner_machine)
  : State(owner_machine, "idle")
{
}


void IdleState::event_occurred(Event *event)
{
  if (event->subsystem == EventSubsystem::WHISKER) {
    ((Behaviour*)_machine)->activate();
    go_to("reverse", (void*)(event->str_value));
  }
}


// -----------------------------------------------------------------------------
//  reverse

ReverseState::ReverseState(StateMachine *owner_machine)
  : State(owner_machine, "reverse")
  , _drive(owner_machine->system()->drive())
  , _timeout(0)
{
}


void ReverseState::enter(void *data)
{
  State::enter(data);
  _data = (char *)data;
  _timeout = millis() + REVERSE_TIME;
  _drive->reverse();
}


void ReverseState::update(uint32_t now)
{
  if (now > _timeout) {
    go_to(strcmp(_data, "left") == 0 ? "right" : "left");
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
  _timeout = millis() + random(1, 10) * 50;
  _drive->pivot_left();
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
  _timeout = millis() + random(1, 10) * 50;
  _drive->pivot_right();
}


void RightState::update(uint32_t now)
{
  if (now > _timeout) {
    ((Behaviour*)_machine)->deactivate();
  }
}


// -----------------------------------------------------------------------------
// wander behaviour

BehaviourWhisker::BehaviourWhisker(System *system)
  :Behaviour(system, "whisker")
{
  add_state(new IdleState(this));
  add_state(new ReverseState(this));
  add_state(new LeftState(this));
  add_state(new RightState(this));
  _initial_state_name = "idle";
  reset();
}
