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
Abstract IR sensor interface/processing
"""


class AbstractIR(object):

    def __init__(self, threshold=20):
        self._data = []
        self._left_pixels = []
        self._center_pixels = []
        self._right_pixels= []
        self._threshold = threshold


    def _setdata(self, new_data):
        if new_data:
            self._data = new_data


    @property
    def threshold(self):
        return self._threshold

    @threshold.setter
    def threshold(self, value):
        self._threshold = value


    def pixels_in(self, ary=None):
        if ary is None:
            ary = self._data
        if not ary or len(ary) == 0:
            return 0
        return sum([len(row) for row in ary])


    @property
    def average(self, ary=None):
        if ary is None:
            ary = self._data
        s = sum([sum(row) for row in ary])
        return s / self.pixels_in(ary)


    @property
    def hottest(self, ary=None):
        if ary is None:
            ary = self._data
        return max([max(x) for x in ary])


    @property
    def coldest(self, ary=None):
        if ary is None:
            ary = self._data
        return min([min(x) for x in ary])


    def percent_above(self, ary=None):
        """Number of pixels above the threshold"""
        if ary is None:
            ary = self._data
        if len(ary) == 0:
            return 0
        count = sum([len([x for x in row if x > self._threshold]) for row in ary])
        return (count * 100.0) // self.pixels_in(ary)


    def temp_printstring(self, ary=None):
        if ary is None:
            ary = self._data
        s = ''
        for row in ary:
            line = ''
            for cell in row:
                line += ' {0:2d}'.format(int(cell))
            s += '{0:s}\n'.format(line)
        return s


    def _convert_temperature(self, t):
        # pylint: disable=multiple-statements
        if   t < 20: return ' '
        elif t < 23: return '.'
        elif t < 25: return '-'
        elif t < 27: return '*'
        elif t < 29: return '+'
        elif t < 31: return 'x'
        elif t < 33: return '%'
        elif t < 35: return '#'
        elif t < 37: return 'X'
        return '&'


    def hot_printstring(self, ary=None):
        if ary is None:
            ary = self._data
        s = ''
        for row in ary:
            line = ''
            for cell in [self._convert_temperature(x) for x in row]:
                line += cell
            s += '{0:s}\n'.format(line)
        return s


    def split(self, cuts):
        sections = []
        for _ in range(len(cuts)):
            sections.append([])
        for row in self._data:
            for i in range(len(cuts)):
                sections[i].append(row[cuts[i][0]:cuts[i][1]])
        return sections


    def _focus_splits(self):
        return []


    @property
    def focus(self):
        left_pixels, center_pixels, right_pixels = self.split(self._focus_splits())
        left_percent = self.percent_above(left_pixels)
        center_percent = self.percent_above(center_pixels)
        right_percent = self.percent_above(right_pixels)

        print('left:')
        print(self.hot_printstring(left_pixels))
        print('center:')
        print(self.hot_printstring(center_pixels))
        print('right:')
        print(self.hot_printstring(right_pixels))

        if left_percent > 10 and left_percent > center_percent and left_percent > right_percent:
            return -1
        elif right_percent > 10 and right_percent > center_percent and right_percent > left_percent:
            return 0
        elif center_percent > 10 and center_percent > left_percent and center_percent > right_percent:
            return 1
        else:
            return None
