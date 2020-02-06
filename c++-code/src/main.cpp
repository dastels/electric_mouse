// Main mouse code
//
// Copyright (c) 2020 Dave Astels

#include <Arduino.h>
#include "drive.h"

RealDrive motors;

void setup()
{
  motors.enable();
}

void loop()
{
  for (int i = 50; i < 256; i += 10) {
    motors.forward(i);
    delay(250);
  }
  for (int i = 255; i > 50; i -= 10) {
    motors.forward(i);
    delay(250);
  }

  for (int i = 50; i < 256; i += 10) {
    motors.pivot_left(i);
    delay(250);
  }

}
