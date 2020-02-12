// Digital debouncer
//
// Copyright (c) 2020 Dave Astels

#include <Arduino.h>
#include "debouncer.h"


Debouncer::Debouncer(Debouncable *db, uint16_t interval_time)
  : debouncable_object(db)
  , interval_millis(interval_time)
  , previous_millis(0)
  , state(0)
{
}


bool Debouncer::update()
{
  // Read the state of the object in a temporary variable.
  bool currentState = debouncable_object->value();

  // If the reading is different from last reading, reset the debounce counter
  if ( currentState != getStateFlag(UNSTABLE_STATE) ) {
    previous_millis = millis();
    toggleStateFlag(UNSTABLE_STATE);
  } else if ( millis() - previous_millis >= interval_millis ) {
    // We have passed the threshold time, so the input is now stable
    // If it is different from last state, set the STATE_CHANGED flag
    if (currentState != getStateFlag(DEBOUNCED_STATE) ) {
      previous_millis = millis();
      toggleStateFlag(DEBOUNCED_STATE);
      setStateFlag(CHANGED_STATE) ;
      durationOfPreviousState = millis() - stateChangeLastTime;
      stateChangeLastTime = millis();
    }
  }
  return  changed();
}


bool Debouncer::read()
{
  return  getStateFlag(DEBOUNCED_STATE);
}


bool Debouncer::fell()
{
  return  !getStateFlag(DEBOUNCED_STATE) && getStateFlag(CHANGED_STATE);
}


bool Debouncer::rose()
{
  return getStateFlag(DEBOUNCED_STATE) && getStateFlag(CHANGED_STATE);
}


unsigned long Debouncer::duration()
{
  return (millis() - stateChangeLastTime);
}


unsigned long Debouncer::previousDuration()
{
  return durationOfPreviousState;
}
