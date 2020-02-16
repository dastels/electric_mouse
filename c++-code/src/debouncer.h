// -*- mode: c++ -*-
// Digital debouncer
//
// Copyright (c) 2020 Dave Astels

#ifndef __DEBOUNCER__
#define __DEBOUNCER__

#include <stdint.h>

#include "debouncable.h"

static const uint8_t DEBOUNCED_STATE = 0b00000001;
static const uint8_t UNSTABLE_STATE  = 0b00000010;
static const uint8_t CHANGED_STATE   = 0b00000100;

class Debouncer
{
  Debouncable *debouncable_object;
  unsigned long previous_millis;
  uint16_t interval_millis;
  uint8_t state;
  unsigned long stateChangeLastTime;
  unsigned long durationOfPreviousState;

  inline void changeState();
  inline void setStateFlag(const uint8_t flag)    { state |= flag; }
  inline void unsetStateFlag(const uint8_t flag)  { state &= ~flag; }
  inline void toggleStateFlag(const uint8_t flag) { state ^= flag; }
  inline bool getStateFlag(const uint8_t flag)    { return((state & flag) != 0); }

 public:
  Debouncer(Debouncable *db, uint16_t interval_time=10);
  bool update();
  bool changed() {return getStateFlag(CHANGED_STATE); }
  bool value() { return  getStateFlag(DEBOUNCED_STATE); }
  bool fell() { return !getStateFlag(DEBOUNCED_STATE) && getStateFlag(CHANGED_STATE); }
  bool rose() { return getStateFlag(DEBOUNCED_STATE) && getStateFlag(CHANGED_STATE); }
  unsigned long duration() { return (millis() - stateChangeLastTime); }
  unsigned long previousDuration() { return durationOfPreviousState; }
};

#endif
