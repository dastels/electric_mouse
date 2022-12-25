// Main mouse code
//
// Copyright (c) 2020 Dave Astels

#include <Arduino.h>
#include "properties.h"
#include "logging.h"
#include "config.h"
#include "system.h"
#include "indicator.h"
#include "drive.h"
#include "accel.h"
#include "ir.h"
#include "range.h"
#include "event.h"
#include "behaviours.h"
#include "behaviour_go_forward.h"
#include "behaviour_wander.h"
#include "behaviour_follow.h"

#include "serial_handler.h"
//#include "sdcard_handler.h"
#include "null_handler.h"

#define LOG_TRANSITIONS (true)
#define DONT_LOG_TRANSITIONS (false)

// Adjust this number for the sensitivity of the 'bump' force
// this strongly depend on the range! for 16G, try 5-10
// for 8G, try 10-20. for 4G try 20-40. for 2G try 40-80
const int bump_threshhold = 40;

// the IR temperature threshold for determining pixels of interest
const float temperature_threshold = 40;

Properties *props;
Logger *logger;


// timeing constants
const unsigned long heartbeat_interval = HEARTBEAT_INTERVAL;
const unsigned long loop_print_interval = LOOP_PRINT_INTERVAL;
const unsigned long accel_update_interval = DEFAULT_ACCEL_UPDATE_INTERVAL;
const unsigned long ir_update_interval = DEFAULT_IR_UPDATE_INTERVAL;
const unsigned long range_update_interval = DEFAULT_RANGE_UPDATE_INTERVAL;
const unsigned long fuel_gauge_update_interval = DEFAULT_FUEL_GAUGE_UPDATE_INTERVAL;


// time tracking variables
unsigned long heartbeat_time = 0;
unsigned long loop_print_time = 0;

// system variables
System *the_system;
Ir *ir;
Debouncer *hotspot;
Debouncer *in_your_face;
Accel *accel;
Range *range;
Indicator *indicator;

Behaviours *behaviours;


// Prefab events to avoid dynamically creating them repeatedly
Event *heartbeat_event;
Event *hotspot_event;
Event *no_hotspot_event;
Event *in_your_face_event;
Event *no_in_your_face_event;
Event *none_focus_event;
Event *left_focus_event;
Event *center_focus_event;
Event *right_focus_event;
Event *close_event;
Event *lux_event;
Event *low_battery_event;
Event *full_battery_event;

void critical_failure(const char *msg)
{
  logger->critical(msg);
  while (1);
}


void initialize_properties()
{
  props = new Properties();
}


void initialize_events()
{
  heartbeat_event       = new Event(EventSubsystem::HEARTBEAT                                           );
  hotspot_event         = new Event(EventSubsystem::IR,      EventType::HOTSPOT,      true              );
  no_hotspot_event      = new Event(EventSubsystem::IR,      EventType::HOTSPOT,      false             );
  in_your_face_event    = new Event(EventSubsystem::IR,      EventType::INYOURFACE,   true              );
  no_in_your_face_event = new Event(EventSubsystem::IR,      EventType::INYOURFACE,   false             );
  none_focus_event      = new Event(EventSubsystem::IR,      EventType::FOCUS,        EventFocus::NONE  );
  left_focus_event      = new Event(EventSubsystem::IR,      EventType::FOCUS,        EventFocus::LEFT  );
  center_focus_event    = new Event(EventSubsystem::IR,      EventType::FOCUS,        EventFocus::CENTER);
  right_focus_event     = new Event(EventSubsystem::IR,      EventType::FOCUS,        EventFocus::RIGHT );
  close_event           = new Event(EventSubsystem::RANGE,   EventType::CLOSE,        (int)0            );
  lux_event             = new Event(EventSubsystem::LUX,     EventType::VALUE,        (float)0.0        );
  low_battery_event     = new Event(EventSubsystem::BATTERY, EventType::LOW                             );
  full_battery_event    = new Event(EventSubsystem::BATTERY, EventType::FULL                            );
}


void initialize_serial()
{
  Serial.begin(115200);
  while (!Serial);
  delay(1000);
}


void initialize_logger(LoggingHandler *handler)
{
  logger = Logger::get_logger(handler);
  logger->set_level(props->log_level());
}

void log_header() {
  logger->info("Electric Mouse version %d.%d", __VERSION_MAJOR__, __VERSION_MINOR__);
}


// If the log-to-serial pop is set enable the serial handler
// If an SD card is installed logging is done to the SD card (possibly as well)

void initialize_logging()
{
  LoggingHandler *handler;
  // LoggingHandler *sd_handler = new SDCardHandler();
  if (props->log_to_serial()) {
    initialize_serial();
    handler = new SerialHandler();
  } else {
    handler = new NullHandler();
  }
  initialize_logger(handler);
  // if (sd_handler->initialized()) {
  //   logger->add_handler(sd_handler);
  // }
  log_header();
  props->log();
}


void initialize_hardware()
{
  the_system = new System(props);
  the_system->set_ir_update_interval(ir_update_interval);
  logger->info("System started");
  indicator = the_system->indicator();
  logger->info("Cached indicator");
  ir = the_system->ir();
  ir->threshold(temperature_threshold);
  logger->info("Cached IR");
  hotspot = the_system->hotspot();
  logger->info("Cached IR hotspot");
  in_your_face = the_system->in_your_face();
  logger->info("Cached IR in your face");
  the_system->power_on();
  logger->info("Powered on");
  accel = the_system->accel();
  // accel->setRange(LIS3DH_RANGE_4_G); // Set Accelerometer Range (2, 4, 8, or 16 G!)
  range = the_system->range();
  // /* Set Accelerometer Click Detection
  // * 0 = turn off click detection & interrupt
  // * 1 = single click only interrupt output
  // * 2 = double click only interrupt output
  // * NOTE: Higher numbers are less sensitive
  // */
  // accel->setClick(1, BUMPTHRESHHOLD);
}


void initialize_behaviours()
{
  logger->info("Initializing behaviours");
  behaviours = new Behaviours(the_system);

  if (!behaviours->add(new BehaviourGoForward(the_system, LOG_TRANSITIONS))) {
    critical_failure("Creating BehaviourGoForward failed");
  }
  logger->info("BehaviourGoForward added");

  if (!behaviours->add(new BehaviourWander(the_system, LOG_TRANSITIONS))) {
    critical_failure("Creating BehaviourWander failed");
  }
  logger->info("BehaviourWander added");

  if (!behaviours->add(new BehaviourFollow(the_system, LOG_TRANSITIONS))) {
    critical_failure("Creating BehaviourFollow failed");
  }
  logger->info("BehaviourFollow added");

  logger->info("Behaviour system initialized, starting");
  behaviours->start();
}


void setup()
{
  initialize_properties();
  initialize_logging();
  logger->info("Logging initialized");
  initialize_events();
  logger->info("Events initialized");
  initialize_hardware();
  logger->info("Hardware initialized");
  initialize_behaviours();
  logger->info("Behaviours initialized");
  logger->info("Setup complete");
  if (!logger->initialized()) {
    while(true);
  }
}


unsigned long now  = 0;

void loop()
{
  now = millis();

  // if (now >= loop_print_time) {
  //   loop_print_time = now + loop_print_interval;
  //   logger->debug_deep("loop");
  // }

  //----------------------------------------------------------------------------
  // First send out a heartbeat when appropriate

  if (now >= heartbeat_time) {
    heartbeat_time = now + heartbeat_interval;
    behaviours->event_occurred(heartbeat_event);
  }

  //----------------------------------------------------------------------------
  // Update things
  the_system->update(now);
  behaviours->update(now);

  //----------------------------------------------------------------------------
  // Handle event triggers


  // if (accel->getClick()) {
  //   // bump event
  // } else
  if (the_system->ir_updated()) {
    if (in_your_face->rose()) {
      indicator->in_your_face(true);
      behaviours->event_occurred(in_your_face_event);
    } else if (in_your_face->fell()) {
      indicator->in_your_face(false);
      behaviours->event_occurred(no_in_your_face_event);
    } else if (hotspot->rose()) {
      behaviours->event_occurred(hotspot_event);
    } else if (hotspot->fell()) {
      behaviours->event_occurred(no_hotspot_event);
    }

    if (hotspot->value()) {     // if a hotspot is present, generate focus events to track it
      EventFocus focus = ir->focus(5);
      logger->debug_deep("IR Focus: %s", name_of_focus(focus));
      indicator->hotspot(focus);
      switch (focus) {
      // case EventFocus::NONE:
      //   behaviours->event_occurred(none_focus_event);
      //   break;
      case EventFocus::LEFT:
        behaviours->event_occurred(left_focus_event);
        break;
      case EventFocus::CENTER:
        behaviours->event_occurred(center_focus_event);
        break;
      case EventFocus::RIGHT:
        behaviours->event_occurred(right_focus_event);
        break;
      default:
        break;
      }
    } else {
      indicator->hotspot(EventFocus::NONE);
    }
  }

  if (the_system->range_updated()) {
    Tof *closest_tof = range->closest();
    if (closest_tof->range() <= CLOSENESS_THRESHOLD) {
      close_event->int_value = closest_tof->range();
      behaviours->event_occurred(close_event);
    }
    lux_event->float_value = range->average_lux();
    behaviours->event_occurred(lux_event);
  }

  if (the_system->fuel_gauge_updated()) {
    if (the_system->low_battery()->rose()) {
      behaviours->event_occurred(low_battery_event);
    }
    if (the_system->full_battery()->rose()) {
      behaviours->event_occurred(full_battery_event);
    }
  }

#ifdef SLOWDOWN
  delay(SLOWDOWN);
#endif
}
