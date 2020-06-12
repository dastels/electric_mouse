// Debouncable class for detecting an IR hotspot
//
// Copyright (c) 2020 Dave Astels

#include "hotspot_debouncable.h"

HotspotDebouncable::HotspotDebouncable(Ir *ir)
  : _ir(ir)
{
}

bool HotspotDebouncable::value()
{
  int percentage = _ir->percent_above(IrSlice::ALL);
  return (percentage >= 10) && (percentage < 75);
}
