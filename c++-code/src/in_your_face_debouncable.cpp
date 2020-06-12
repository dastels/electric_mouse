// Debouncable class for detecting something very close
//
// Copyright (c) 2020 Dave Astels

#include "in_your_face_debouncable.h"

InYourFaceDebouncable::InYourFaceDebouncable(Ir *ir)
  : _ir(ir)
{
}

bool InYourFaceDebouncable::value()
{
  return _ir->percent_above(IrSlice::ALL) >= 75;
}
