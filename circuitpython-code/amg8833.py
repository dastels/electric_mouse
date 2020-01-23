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
AMG8833 interface/processing
"""

import adafruit_amg88xx
from ir import AbstractIR

class AMG8833Sensor(AbstractIR):

    def __init__(self, i2c, threshold=20):
        super(AMG8833Sensor, self).__init__(threshold)
        self._sensor = adafruit_amg88xx.AMG88XX(i2c)


    def update(self):
        """Read the sensor and cache the pixel values"""
        self._setdata(self._sensor.pixels)


    @property
    def pixel_count(self):
        return 64


    def _focus_splits(self):
        return [(0, 3), (3, 5), (5, 8)]
