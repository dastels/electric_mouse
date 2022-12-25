// IR sensor interface
//
// Copyright (c) 2020 Dave Astels

#include <math.h>
#include "logging.h"
#include "ir.h"

extern Logger *logger;

int starts[] = {0, 0, 0, 0};
int widths[] = {32, 0, 0, 0};
int ends[] = {32, 0, 0, 32};

const char *name_of(IrSlice slice)
{
  switch (slice) {
  case IrSlice::ALL:
    return "ALL";
  case IrSlice::LEFT:
    return "LEFT";
  case IrSlice::CENTER:
    return "CENTER";
  case IrSlice::RIGHT:
    return "RIGHT";
  }
  return "ERROR";
}


Ir::Ir()
  : _left_width(0)
  , _right_width(0)
  , _sensor(new Adafruit_MLX90640())
  , _good_frame(false)
  , _update_log_count(0)
  , _threshold(40)
{
  split(10, 10);
  logger->info("Adafruit MLX90640 Camera");
  if (!_sensor->begin(MLX90640_I2CADDR_DEFAULT, &Wire1)) {
    logger->critical("MLX90640 not found!");
    while (true);
  }
  logger->info("Found Adafruit MLX90640");

  logger->info("Creating IR");
  _sensor->setMode(MLX90640_CHESS);
  logger->info("Set mode");
  _sensor->setResolution(MLX90640_ADC_18BIT);
  logger->info("Set resolution");
  _sensor->setRefreshRate(MLX90640_8_HZ);
  logger->info("Set refresh rate");
  Wire.setClock(1000000); // max 1 MHz
  logger->info("Set Wire speed");
  logger->info("Finished creating IR");
}


void Ir::threshold(float th)
{
  _threshold = th;
}


int Ir::pixels_in(IrSlice slice)
{
  return width_of(slice) * 24;
}


void Ir::split(int left_width, int right_width)
{
  _left_width = left_width;
  _right_width = right_width;
  _center_size_scaling_factor = pixels_in(IrSlice::CENTER) / pixels_in(IrSlice::CENTER);

  starts[int(IrSlice::CENTER)] = left_width;
  starts[int(IrSlice::RIGHT)] = 32 - right_width;

  ends[int(IrSlice::LEFT)] = left_width;
  ends[int(IrSlice::CENTER)] = 32 - _right_width;

  widths[int(IrSlice::LEFT)] = left_width;
  widths[int(IrSlice::CENTER)] = 32 - (_right_width + _left_width);
  widths[int(IrSlice::RIGHT)] = right_width;
}


int Ir::start_of(IrSlice slice)
{
  return starts[int(slice)];
}


int Ir::end_of(IrSlice slice)
{
  return ends[int(slice)];
}


int Ir::width_of(IrSlice slice)
{
  return widths[int(slice)];
}


float Ir::average(IrSlice slice)
{
  int start = start_of(slice);
  int end = end_of(slice);
  float sum = 0;
  for (int y = 0; y < 24; y++) {
    for (int x = start; x < end; x++) {
      sum += _frame[y * 32 + x];
    }
  }
  return sum / (width_of(slice) * 24);
}


float Ir::hottest(IrSlice slice)
{
  int start = start_of(slice);
  int end = end_of(slice);
  float hot = 0;
  for (int y = 0; y < 24; y++) {
    for (int x = start; x < end; x++) {
      if (_frame[y * 32 + x] > hot) {
        hot = _frame[y * 32 + x];
      }
    }
  }
  return hot;
}


float Ir::coldest(IrSlice slice)
{
  int start = start_of(slice);
  int end = end_of(slice);
  float cold = 0;
  for (int y = 0; y < 24; y++) {
    for (int x = start; x < end; x++) {
      if (_frame[y * 32 + x] < cold) {
        cold = _frame[y * 32 + x];
      }
    }
  }
  return cold;
}


int Ir::percent_above(IrSlice slice)
{
  int start = start_of(slice);
  int end = end_of(slice);
  int count = 0;
  for (int y = 0; y < 24; y++) {
    for (int x = start; x < end; x++) {
      if (_frame[y * 32 + x] > _threshold) {
        count++;
      }
    }
  }
  int above = (count * 100) / pixels_in(slice);
  logger->debug_deep("Percent of %s above threshold is %d", name_of(slice), above);
  return above;
}


int Ir::max_of_three(int v1, int v2, int v3)
{
  int m1 = v1 > v2 ? v1 : v2;
  return m1 > v3 ? m1 : v3;
}



EventFocus Ir::focus(int percentage_threshold)
{
  int left_percent = percent_above(IrSlice::LEFT);
  int center_percent = _center_size_scaling_factor * percent_above(IrSlice::CENTER);
  int right_percent = percent_above(IrSlice::RIGHT);
  int max_percent = max_of_three(left_percent, center_percent, right_percent);
  if (max_percent >= percentage_threshold) {
    if (center_percent == max_percent) {
      return EventFocus::CENTER;
    } else if (left_percent == max_percent) {
      return EventFocus::LEFT;
    } else if (right_percent == max_percent) {
      return EventFocus::RIGHT;
    }
  }
  return EventFocus::NONE;
}


char frame_log_buf[97];        // enough for one row of output: 32 * 3 + 1

void Ir::log_if_time()
{
  _update_log_count++;
  if (_good_frame && _update_log_count == 8 && logger->get_level() == LogLevel::DEBUG_DEEP) {
    logger->debug_deep("Logging IR frame");
    _update_log_count = 0;
    int buf_start;
    for (int y = 0; y < 24; y++) {
      buf_start = 0;
      for (int x = 0; x < 32; x++) {
        int number_printed = sprintf(frame_log_buf + buf_start, "%2d ", (int)_frame[y * 32 + x]);
        if (number_printed != 3) {
          logger->error("Bad result from sprintf: %d", number_printed);
        }
        buf_start += 3;
      }
      logger->debug_deep("Line %02d: %s", y, frame_log_buf);
    }
  }
}


void Ir::update()
{
  // status of -1 means a NACK occurred during the communication
  // -8 means that the data could not be acquired, probably because the I2C frequency is too low.
  int status = _sensor->getFrame(_frame);
  _good_frame = status == 0;
  if (!_good_frame) {
    logger->error("Reading MLX90640 frame failed with: %d", status);
  }
  // log_if_time();
}
