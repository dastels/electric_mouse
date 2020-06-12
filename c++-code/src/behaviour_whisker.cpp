// Go-Forward behaviour
//
// Copyright (c) 2020 Dave Astels

#include "behaviour_whisker.h"
#include "drive.h"

// Reverse until whisker is release then this much more to try to clear the obstacle
const unsigned long EXTRA_REVERSE_TIME = 200;

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
    const char *_side;

  public:
    ReverseState(StateMachine *owner_machine);
    void enter(void *data);
    void event_occurred(Event *event);
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
  if (event->subsystem == EventSubsystem::WHISKER && event->bool_value) {
    ((Behaviour*)_machine)->activate();
    go_to("reverse", (void*)((event->type == EventType::LEFT) ? "right" : "left"));
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

  _drive->stop();
  _side = (char *)data;
  _drive->reverse();
  _timeout = 0;
}


void ReverseState::event_occurred(Event *event)
{
  if (event->subsystem == EventSubsystem::WHISKER && !event->bool_value) {
    _timeout = millis() + EXTRA_REVERSE_TIME;
  }
}


void ReverseState::update(uint32_t now)
{
  // only turn after whisker has been released AND a bit of extra time has passed
  if (_timeout > 0 && now > _timeout) {
    _drive->stop();
    go_to(_side);
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

BehaviourWhisker::BehaviourWhisker(System *system, bool should_log_transitions)
  :Behaviour(system, "whisker", should_log_transitions)
{
  add_state(new IdleState(this));
  add_state(new ReverseState(this));
  add_state(new LeftState(this));
  add_state(new RightState(this));
  _initial_state_name = "idle";
  reset();
}
