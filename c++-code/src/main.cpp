// Main mouse code
//
// Copyright (c) 2020 Dave Astels

#include <Arduino.h>
#include "system.h"
#include "behaviours.h"
#include "behaviour_go_forward.h"
#include "behaviour_wander.h"

// timeing constants
const unsigned long heartbeat_interval = 100; // how often a heartbeat event is published
const unsigned long update_interval = 10; // how often an update is made

// time tracking variables
unsigned long heartbeat_time = 0;
unsigned long update_time = 0;

System *the_system;
Behaviours *behaviours;
Event *heartbeat_event;

void setup()
{
  heartbeat_event = new Event("hartbeat");
  the_system = new System(false);
  behaviours = new Behaviours(the_system);
  behaviours->add(new BehaviourGoForward(the_system));
  behaviours->add(new BehaviourWander(the_system));
  the_system->power_on();
  behaviours->start();
}

unsigned long now  = 0;

void loop()
{
  now = millis();

  if (now >= update_time) {
    update_time = now + update_interval;
    the_system->update(now);
    behaviours->update(now);
  }

  if (now >= heartbeat_time) {
    heartbeat_time = now + heartbeat_interval;
    behaviours->event_occurred(heartbeat_event);
  }
}
