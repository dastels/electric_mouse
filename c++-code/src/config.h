// -*- mode: c++ -*-
// Configuration
//
// Copyright (c) 2020 Dave Astels

#ifndef __CONFIG_H__
#define __CONFIG_H__

#define __VERSION_MAJOR__ (0)
#define __VERSION_MINOR__ (1)

//#define SLOWDOWN (500)
#define CLOSENESS_THRESHOLD (50) // 50 mm is too close

#define HEARTBEAT_INTERVAL (1000) // how often a heartbeat event is published
#define LOOP_PRINT_INTERVAL (1000)

#define DEFAULT_ACCEL_UPDATE_INTERVAL (250)         // check the acceleromoter four times a second
#define DEFAULT_IR_UPDATE_INTERVAL (1000)           // check front facing IR camera every second
#define DEFAULT_RANGE_UPDATE_INTERVAL (500)         //  check the 8 direction range sensor twice a second
#define DEFAULT_FUEL_GAUGE_UPDATE_INTERVAL (300000) // check the battery every 5 minutes

#endif
