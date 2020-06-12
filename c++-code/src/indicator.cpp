// -*- mode: c++ -*-
// Indicator strip interface
//
// Copyright (c) 2020 Dave Astels

#include "indicator.h"

const int left_whisker_pixel = 7;
const int right_whisker_pixel = 0;
const int left_drive_pixel = 6;
const int right_drive_pixel = 1;
const int left_ir_pixel = 4;
const int right_ir_pixel = 3;


Indicator::Indicator()
  : _strip(new Adafruit_NeoPixel(8, 5, NEO_GRB + NEO_KHZ800))
{
  _strip->begin();
}


void Indicator::begin()
{
  clear();
  show();
}


void Indicator::clear()
{
  fill(0x00, 0x00, 0x00);
}


void Indicator::fill(uint8_t red, uint8_t green, uint8_t blue)
{
  _strip->fill(red, green, blue);
  show();
}


void Indicator::show()
{
  _strip->show();
}


void Indicator::setPixelColour(uint8_t pixel_number, uint8_t red, uint8_t green, uint8_t blue)
{
  _strip->setPixelColor(pixel_number, red, green, blue);
  show();
}


void Indicator::left_whisker(bool on_off)
{
  setPixelColour(left_whisker_pixel, (on_off ? 0x80 : 0x00), 0x00, 0x00);
}


void Indicator::right_whisker(bool on_off)
{
  setPixelColour(right_whisker_pixel, (on_off ? 0x80 : 0x00), 0x00, 0x00);
}


void Indicator::show_speed(int pixel, int speed)
{
  if (speed >= 0) {
    setPixelColour(pixel, 0x00, speed, 0x00);
  } else {
    setPixelColour(pixel, speed, 0x00, 0x00);
  }
}


void Indicator::left_drive(int speed)
{
  show_speed(left_drive_pixel, speed);
}


void Indicator::right_drive(int speed)
{
  show_speed(right_drive_pixel, speed);
}


void Indicator::in_your_face(bool present)
{
  setPixelColour(left_ir_pixel, (present ? 0xff : 0x00), 0x00, 0x00);
  setPixelColour(right_ir_pixel, (present ? 0xff : 0x00), 0x00, 0x00);
}


void Indicator::hotspot(EventFocus focus)
{
  switch (focus) {
  case EventFocus::NONE:
    setPixelColour(left_ir_pixel, 0x00, 0x00, 0x00);
    setPixelColour(right_ir_pixel, 0x00, 0x00, 0x00);
    break;
  case EventFocus::LEFT:
    setPixelColour(left_ir_pixel, 0x00, 0x00, 0x80);
    setPixelColour(right_ir_pixel, 0x00, 0x00, 0x00);
    break;
  case EventFocus::CENTER:
    setPixelColour(left_ir_pixel, 0x00, 0x00, 0x80);
    setPixelColour(right_ir_pixel, 0x00, 0x00, 0x80);
    break;
  case EventFocus::RIGHT:
    setPixelColour(left_ir_pixel, 0x00, 0x00, 0x00);
    setPixelColour(right_ir_pixel, 0x00, 0x00, 0x80);
    break;
  }
}
