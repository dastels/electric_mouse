// -*- mode: c++ -*-
// Debouncable class for detecting an IR hotspot
//
// Copyright (c) 2020 Dave Astels

#ifndef __HOT_SPOTDEBOUNCABLE_H__
#define __HOT_SPOTDEBOUNCABLE_H__

#include "ir.h"
#include "debouncable.h"

class HotspotDebouncable: public Debouncable
{
  Ir *_ir;

public:
  HotspotDebouncable(Ir *ir);
  bool value();
};

#endif
