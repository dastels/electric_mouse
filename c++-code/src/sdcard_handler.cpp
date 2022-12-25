// SD card logging handler
//
// Copyright (c) 2020 Dave Astels

// #include <SPI.h>
// #include <SD.h>
// #include "sdcard_handler.h"
// 2022-07-17 19:34
// #define LOG_FNAME ("log.txt")

// SDCardHandler::SDCardHandler()
//   : LoggingHandler()
//   , _chip_select(19)
// {
//   _success = SD.begin(_chip_select);
//   if (_success && SD.exists(LOG_FNAME)) {
//     SD.remove(LOG_FNAME);
//   }
// }


// void SDCardHandler::emit(const char *level_name, const char *msg)
// {
//   if (_success) {
//     File log_file = SD.open(LOG_FNAME, FILE_WRITE);
//     log_file.println(format(level_name, msg));
//     log_file.close();
//   }
// }
