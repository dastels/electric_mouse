// -*- mode: c++ -*-
// Debouncable class for detecting an IR hotspot
 //
// Copyright (c) 2020 Dave Astels

#ifndef __IN_YOUR_FACEDEBOUNCABLE_H__
#define __IN_YOUR_FACEDEBOUNCABLE_H__

#include "ir.h"
#include "debouncable.h"

class InYourFaceDebouncable: public Debouncable
{
  Ir *_ir;

 public:
  InYourFaceDebouncable(Ir *ir);
  bool value();
};

#endif
