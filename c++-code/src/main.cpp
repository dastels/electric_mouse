// Main mouse code
//
// Copyright (c) 2020 Dave Astels

#include <Arduino.h>
#include "system.h"
#include "behaviours.h"
#include "behaviour_go_forward.h"

System *the_system;
Behaviours *behaviours;

bool led = true;

void setup()
{
  the_system = new System(false);
  behaviours = new Behaviours(the_system);
  behaviours->add(new BehaviourGoForward(the_system));
  the_system->power_on();
  behaviours->start();
}

void loop()
{
  led = !led;
  the_system->update(0);
  behaviours->update(0);
  delay(1000);
}
