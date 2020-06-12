// -*- mode: c++ -*-
// Hardware interface
//
// Copyright (c) 2020 Dave Astels

#ifndef __PROXIMITY_H__
#define __PROXIMITY_H__

class Proximity
{
  const char * _name;
  const uint8_t _address;

 public:
  Proximity(const char *name, const uint8_t address);
  void interrupt_threshold(int threshold);
  void interrupt_callback(void (*isr)());
  int light();
  int distance();
};


#endif
