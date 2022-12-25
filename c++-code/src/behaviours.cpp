// Behaviour management class
//
// Copyright (c) 2020 Dave Astels

#include "logging.h"
#include "system.h"
#include "behaviours.h"

extern Logger *logger;
const unsigned long update_interval = 10; // how often to update


Behaviours::Behaviours(System *sys)
  : _number_of_behaviours(0)
  , _system(sys)
  , _update_time(0)
{
}

bool Behaviours::add(Behaviour *behaviour)
{
  Behaviour *subsumed = nullptr;
  if (_number_of_behaviours == MAX_BEHAVIOURS) return false;
  if (!empty()) {
    subsumed = _behaviours[_number_of_behaviours - 1];
  }
  behaviour->subsume(subsumed);
  _behaviours[_number_of_behaviours++] = behaviour;
  return true;
}


void Behaviours::start()
{
  if (empty()) return;
  _behaviours[0]->activate();
}


void Behaviours::update(uint32_t now)
{
  if (now >= _update_time) {
    _update_time = now + update_interval;
    for (int8_t index = _number_of_behaviours - 1; index >= 0; index--) {
      _behaviours[index]->update(now);
      if (_behaviours[index]->active()) {
        return;
      }
    }
  }
}


void Behaviours::event_occurred(Event *event)
{
  logger->debug_deep("%s", event->to_string());
  for (int8_t index = _number_of_behaviours - 1; index >= 0; index--) {
    _behaviours[index]->event_occurred(event);
    if (_behaviours[index]->active()) {
      logger->debug_deep("%s processed by %s", event->to_string(), _behaviours[index]->name());
      return;
    }
  }

}
