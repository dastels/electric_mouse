"""
The MIT License (MIT)

Copyright (c) 2020 Dave Astels

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

--------------------------------------------------------------------------------
The system: initialization and caching
"""


import board
from digitalio import DigitalInOut, Direction
import audioio
import busio
import adafruit_lis3dh
#from amg8833 import AMG8833Sensor
from mlx90640 import MLX90640Sensor
import adafruit_logging as logging
from adafruit_debouncer import Debouncer
import neopixel

logger = logging.getLogger('mouse')
logger.setLevel(logging.DEBUG)

class System(object):

    def __init__(self, drive, debounce_update_interval=0.01, ir_update_interval=0.25):
        i2c = busio.I2C(board.SCL, board.SDA)

        # Drive system
        self._drive = drive

        # power control of the propwing
        self._propwing_power = DigitalInOut(board.D10)
        self._propwing_power.direction = Direction.OUTPUT

        # whiskers
        self._right_whisker = Debouncer(board.D0)
        self._left_whisker = Debouncer(board.D1)

        # powerboost low voltage indicator
        self._low_voltage = Debouncer(board.A4)

        # propwing switch
        self._switch = Debouncer(board.D9)

        # audio amp/speaker
        self._audio = audioio.AudioOut(board.A0)

        # accelerometer, 4G range:
        int1 = DigitalInOut(board.D6)
        self._accel = adafruit_lis3dh.LIS3DH_I2C(i2c, int1=int1)
        self._accel.range = adafruit_lis3dh.RANGE_4_G
        self._accel.set_tap(1, 100)

        self._propwing_power.value = True

        # Neopixels
        self._indicator = neopixel.NeoPixel(board.D5, 1, brightness=0.3, auto_write=True)

        # IR sensor
        # self._ir = AMG8833Sensor(i2c)
        self._ir = MLX90640Sensor(i2c)
        self._hotspot_debouncer = Debouncer(lambda: self._ir.percent_above() > 10)
        self._inyourface_debouncer = Debouncer(lambda: self._ir.percent_above() > 75)

        self._debounce_update_interval = debounce_update_interval
        self._ir_update_interval = ir_update_interval

        self._debounce_update_time = 0
        self._ir_update_time = 0


    def update(self, now):
        self._left_whisker.update()
        self._right_whisker.update()
        self._low_voltage.update()
        self._hotspot_debouncer.update()
        self._inyourface_debouncer.update()
        if now > self._ir_update_time:
            self._ir_update_time = now + self._ir_update_interval
            self._ir.update()
            # print(self._ir.hot_printstring())


    def power_off(self):
        self._drive.disable()
        self._propwing_power.value = False


    def power_on(self):
        self._drive.enable()
        self._propwing_power.value = True


    @property
    def drive(self):
        return self._drive


    @property
    def right_whisker(self):
        return self._right_whisker


    @property
    def left_whisker(self):
        return self._left_whisker


    @property
    def hotspot(self):
        return self._hotspot_debouncer


    @property
    def in_your_face(self):
        return self._inyourface_debouncer


    @property
    def low_voltage(self):
        return self._low_voltage


    @property
    def switch(self):
        return self._switch


    @property
    def ir(self):
        return self._ir


    @property
    def accel(self):
        return self._accel

    def indicate(self, colour):
        self._indicator[0] = colour
