// Follow behaviour
//
// Copyright (c) 2020 Dave Astels

#include "behaviour_follow.h"
#include "drive.h"

// local states

namespace {

  class IdleState: public State
  {
  public:
    IdleState(StateMachine *owner_machine);
    void enter(void *data);
    void event_occurred(Event *event);
  };


  class TrackState: public State
  {
  public:
    TrackState(StateMachine *owner_machine);
    void enter(void *data);
    void event_occurred(Event *event);
  };


  class LeftState: public State
  {
    Drive *_drive;

  public:
  LeftState(StateMachine *owner_machine);
    void enter(void *data);
    void event_occurred(Event *event);
  };


  class RightState: public State
  {
    Drive *_drive;

  public:
    RightState(StateMachine *owner_machine);
    void enter(void *data);
    void event_occurred(Event *event);
  };

}


// -----------------------------------------------------------------------------
// idle

IdleState::IdleState(StateMachine *owner_machine)
  : State(owner_machine, "idle")
{
}


void IdleState::enter(void *data)
{
  _machine->system()->indicate(0x00, 0x00, 0x00);
}


void IdleState::event_occurred(Event *event)
{
  State::event_occurred(event);
  if (event->subsystem == EventSubsystem::IR && event->type == EventType::HOTSPOT) {
    if (event->bool_value) {
      ((Behaviour*)_machine)->activate();
      go_to("track");
    }
  }
}


// -----------------------------------------------------------------------------
// track

TrackState::TrackState(StateMachine *owner_machine)
  : State(owner_machine, "track")
{
}


void TrackState::enter(void *data)
{
  _machine->system()->indicate(0xFF, 0xFF, 0xFF);
}


void TrackState::event_occurred(Event *event)
{
  State::event_occurred(event);
  if (event->subsystem == EventSubsystem::IR) {
    if (event->type == EventType::HOTSPOT && !event->bool_value) {
      ((Behaviour*)_machine)->deactivate();
    } else if (event->type == EventType::FOCUS) {
      switch (event->focus_value) {
      case EventFocus::LEFT:
        go_to("left");
        break;
      case EventFocus::RIGHT:
        go_to("right");
        break;
      case EventFocus::CENTER:
        ((Behaviour*)_machine)->deactivate();
        break;
      default:
        break;
      }
    }
  }
}


// -----------------------------------------------------------------------------
//  left

LeftState::LeftState(StateMachine *owner_machine)
  : State(owner_machine, "left")
  , _drive(owner_machine->system()->drive())
{
}


void LeftState::enter(void *data)
{
  State::enter(data);
  _machine->system()->indicate(0x00, 0xFF, 0x00);
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


void LeftState::event_occurred(Event *event)
{
  State::event_occurred(event);
  if (event->subsystem == EventSubsystem::IR) {
    if (event->type == EventType::HOTSPOT && !event->bool_value) {
      ((Behaviour*)_machine)->deactivate();
    } else if (event->type == EventType::FOCUS) {
      switch (event->focus_value) {
      case EventFocus::LEFT:
        go_to("left");
        break;
      case EventFocus::RIGHT:
        go_to("right");
        break;
      case EventFocus::CENTER:
      case EventFocus::NONE:
        ((Behaviour*)_machine)->deactivate();
        break;
      default:
        break;
      }
    }
  }
}


// -----------------------------------------------------------------------------
//  right

RightState::RightState(StateMachine *owner_machine)
  : State(owner_machine, "right")
  , _drive(owner_machine->system()->drive())
{
}


void RightState::enter(void *data)
{
  State::enter(data);
  _machine->system()->indicate(0x00, 0x00, 0xFF);
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


void RightState::event_occurred(Event *event)
{
  State::event_occurred(event);
  if (event->subsystem == EventSubsystem::IR) {
    if (event->type == EventType::HOTSPOT && !event->bool_value) {
      ((Behaviour*)_machine)->deactivate();
    } else if (event->type == EventType::FOCUS) {
      switch (event->focus_value) {
      case EventFocus::LEFT:
        go_to("left");
        break;
      case EventFocus::RIGHT:
        go_to("right");
        break;
      case EventFocus::CENTER:
      case EventFocus::NONE:
        ((Behaviour*)_machine)->deactivate();
        break;
      default:
        break;
      }
    }
  }
}


// -----------------------------------------------------------------------------
// follow behaviour

BehaviourFollow::BehaviourFollow(System *system)
  :Behaviour(system, "follow")
{
  add_state(new IdleState(this));
  add_state(new TrackState(this));
  add_state(new LeftState(this));
  add_state(new RightState(this));
  _initial_state_name = "idle";
  reset();
}
