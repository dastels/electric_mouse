// -*- mode: c++ -*-
// Abstract debouncable base class
//
// Copyright (c) 2020 Dave Astels

#ifndef __DEBOUNCABLE_H__
#define __DEBOUNCABLE_H__


class Debouncable
{
 public:
  virtual bool value() = 0;
};

#endif
