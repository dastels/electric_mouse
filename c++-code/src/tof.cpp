// -*- mode: c++ -*-
// TOF distance sensor interface
//
// Copyright (c) 2020 Dave Astels

#include "logging.h"
#include <tof.h>

extern Logger *logger;

#define TCAADDR 0x70

void Tof::tca_select(uint8_t i) {
  if (i > 7) {
    logger->critical("Bad multiplexor channel: %d", i);
  }

  Wire1.beginTransmission(TCAADDR);
  Wire1.write(1 << i);
  Wire1.endTransmission();
}


Tof::Tof(TofDirection direction, int channel)
  :_direction(direction)
  ,_channel(channel)
  ,_vl(new Adafruit_VL6180X())
  ,_cached_range(0)
  ,_cached_lux(0)
{
  tca_select(channel);
  _vl->begin(&Wire1);
}


const char *Tof::direction_string()
{
  switch (_direction) {
  case TofDirection::AHEAD_LEFT:
    return "AHEAD_LEFT";
  case TofDirection::AHEAD:
    return "AHEAD";
  case TofDirection::AHEAD_RIGHT:
    return "AHEAD_RIGHT";
  case TofDirection::RIGHT:
    return "RIGHT";
  case TofDirection::BEHIND_RIGHT:
    return "BEHIND_RIGHT";
  case TofDirection::BEHIND:
    return "BEHIND";
  case TofDirection::BEHIND_LEFT:
    return "BEHIND_LEFT";
  case TofDirection::LEFT:
    return "LEFT";
  default:
    logger->critical("Unknown ToF direction");
    return nullptr;
  }
}


void Tof::report_status(uint8_t status)
{
  if  ((status >= VL6180X_ERROR_SYSERR_1) && (status <= VL6180X_ERROR_SYSERR_5)) {
    logger->error("ToF %s: System error", direction_string());
  }
  else if (status == VL6180X_ERROR_ECEFAIL) {
    logger->error("ToF %s: ECE failure", direction_string());
  }
  else if (status == VL6180X_ERROR_NOCONVERGE) {
    logger->info("ToF %s: No convergence", direction_string());
  }
  else if (status == VL6180X_ERROR_RANGEIGNORE) {
    logger->info("ToF %s: Ignoring range", direction_string());
  }
  else if (status == VL6180X_ERROR_SNR) {
    logger->info("ToF %s: Signal/Noise error", direction_string());
  }
  else if (status == VL6180X_ERROR_RAWUFLOW) {
    logger->info("ToF %s: Raw reading underflow", direction_string());
  }
  else if (status == VL6180X_ERROR_RAWOFLOW) {
    logger->info("ToF %s: Raw reading overflow", direction_string());
  }
  else if (status == VL6180X_ERROR_RANGEUFLOW) {
    logger->info("ToF %s: Range reading underflow", direction_string());
  }
  else if (status == VL6180X_ERROR_RANGEOFLOW) {
    logger->info("ToF %s: Range reading overflow", direction_string());
  }
}


void Tof::update()
{
  tca_select(_channel);
  uint8_t range = _vl->readRange();
  uint8_t status = _vl->readRangeStatus();
  if (status != 0) {
    report_status(status);
  } else {
    _cached_range = range;
  }
  _cached_lux = _vl->readLux(VL6180X_ALS_GAIN_5);
}
