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
MLX90640 interface/processing
"""

from ir import AbstractIR
from adafruit_mlx90640 import MLX90640, RefreshRate


class MLX90640Sensor(AbstractIR):

    def __init__(self, i2c, threshold=20):
        super(MLX90640Sensor, self).__init__(threshold)
        self._sensor = MLX90640(i2c)
        self._sensor.refresh_rate = RefreshRate.REFRESH_4_HZ


    def update(self):
        """Read the sensor and cache the pixel values"""
        frame = [0] * 768
        try:
            self._sensor.getFrame(frame)
        except ValueError:                     # these happen, no biggie - retry
            return
        data = []
        for h in range(24):
            index = h * 32
            row = frame[index:index + 32]
            row.reverse()
            data.append(row)
        data.reverse()
        self._setdata(data)


    @property
    def pixel_count(self):
        return 768                        #24x32


    def _focus_splits(self):
        return [(0, 10), (10, 22), (22, 32)]
