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
Behaviour collection/management
"""

import adafruit_logging as logging
logger = logging.getLogger('mouse')

class Behaviours(object):

    def __init__(self):
        self._behaviours = []
        self._already_started = False


    def add(self, behaviour):
        subsumed = self._behaviours[0] if len(self._behaviours) > 0 else None
        behaviour.subsume(subsumed)
        self._behaviours.insert(0, behaviour)


    def start(self):
        if self._already_started:
            return
        self._behaviours[-1].activate()
        self._already_started = True


    def update(self):
        self.start()
        for behaviour in self._behaviours:
            behaviour.update()
            if behaviour.active:
                return


    def event_occurred(self, event):
        self.start()
        for behaviour in self._behaviours:
            behaviour.event_occurred(event)
            if behaviour.active:
                return
