// Debouncable class for detecting an IR hotspot
//
// Copyright (c) 2020 Dave Astels

#include "hotspot_debouncable.h"

HotspotDebouncable::HotspotDebouncable(Ir *ir)
  :_ir(ir)
{
}

bool HotspotDebouncable::value()
{
  return _ir->percent_above(IrSlice::ALL) > 10;
}
