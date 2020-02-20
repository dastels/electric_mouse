// Main mouse code
//
// Copyright (c) 2020 Dave Astels

#include <Arduino.h>
#include "logging.h"
#include "config.h"
#include "system.h"
#include "drive.h"
#include "ir.h"
#include "behaviours.h"
#include "behaviour_go_forward.h"
#include "behaviour_wander.h"
#include "behaviour_whisker.h"
#include "behaviour_follow.h"

Logger *logger;

// motor trim
const int left_trim = 10;
const int right_trim = 0;


// timeing constants
const unsigned long heartbeat_interval = 100; // how often a heartbeat event is published
const unsigned long update_interval = 1; // how often an update is made


// time tracking variables
unsigned long heartbeat_time = 0;
unsigned long update_time = 0;


// system variables
System *the_system;
Debouncer *left_whisker;
Debouncer *right_whisker;
Ir *ir;
Debouncer *hotspot;
Debouncer *in_your_face;
Behaviours *behaviours;


// Prefab events to avoid dynamically creating them repeatedly
Event *heartbeat_event;
Event *left_whisker_event;
Event *right_whisker_event;
Event *hotspot_event;
Event *no_hotspot_event;
Event *in_your_face_event;
Event *no_in_your_face_event;

Event *make_heartbeat_event() {
  Event *e = new Event;
  e->subsystem = EventSubsystem::HEARTBEAT;
  return e;
}


Event *make_whisker_event(EventType type) {
  Event *e = new Event;
  e->subsystem = EventSubsystem::WHISKER;
  e->type = type;
  e->str_value = (char*)((type == EventType::LEFT) ? "left" : "right");
  return e;
}


Event *make_ir_event(EventType t, bool v) {
  Event *e = new Event;
  e->subsystem = EventSubsystem::IR;
  e->type = t;
  e->bool_value = v;
  return e;
}


void initialize_events()
{
  heartbeat_event = make_heartbeat_event();
  left_whisker_event = make_whisker_event(EventType::LEFT);
  right_whisker_event = make_whisker_event(EventType::RIGHT);
  hotspot_event = make_ir_event(EventType::HOTSPOT, true);
  no_hotspot_event = make_ir_event(EventType::HOTSPOT, false);
  in_your_face_event = make_ir_event(EventType::INYOURFACE, true);
  no_in_your_face_event = make_ir_event(EventType::INYOURFACE, false);
}


void initialize_serial()
{
  Serial.begin(115200);
  while (!Serial);
  delay(1000);
}


void initialize_logger()
{
  logger = Logger::get_logger();
  logger->info("Connected.");
  logger->info("Electric Mouse version %f", __MOUSE_VERSION__);
}


void initialize_hardware()
{
  the_system = new System(false);
  the_system->drive()->trim(left_trim, right_trim);
  left_whisker = the_system->left_whisker();
  right_whisker = the_system->right_whisker();
  // ir = the_system->ir();
  // hotspot = the_system->hotspot();
  // in_your_face = the_system->in_your_face();
  the_system->power_on();
}


void initialize_behaviours()
{
  behaviours = new Behaviours(the_system);
  behaviours->add(new BehaviourGoForward(the_system));
  behaviours->add(new BehaviourWander(the_system));
  behaviours->add(new BehaviourWhisker(the_system));
  // behaviours->add(new BehaviourFollow(the_system));
  behaviours->start();
}


void setup()
{
  initialize_serial();
  initialize_logger();
  initialize_events();
  initialize_hardware();
  the_system->indicate(0x00, 0x00, 0x88);
  initialize_behaviours();
  the_system->indicate(0x00, 0x88, 0x88);
}


unsigned long now  = 0;

void loop()
{
  the_system->indicate(0x00, 0x00, 0x00);

  now = millis();

  if (now >= heartbeat_time) {
    heartbeat_time = now + heartbeat_interval;
    behaviours->event_occurred(heartbeat_event);
  }

  if (now >= update_time) {
    update_time = now + update_interval;
    the_system->update(now);
    behaviours->update(now);

    // Whiskers
    if (right_whisker->fell()) {
      behaviours->event_occurred(right_whisker_event);
    } else if (left_whisker->fell()) {
      behaviours->event_occurred(left_whisker_event);
    }

    // ir
    // if (in_your_face->rose()) {
    //   behaviours->event_occurred(in_your_face_event);
    // } else if (in_your_face->fell()) {
    //   behaviours->event_occurred(no_in_your_face_event);
    // }

    // if (hotspot->rose()) {
    //   behaviours->event_occurred(hotspot_event);
    // } else if (hotspot->fell()) {
    //   behaviours->event_occurred(no_hotspot_event);
    // }
  }
}
