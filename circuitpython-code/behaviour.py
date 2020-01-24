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
Base behaviour
"""

import adafruit_logging as logging
from state_machine import StateMachine

logger = logging.getLogger('mouse')

class Behaviour(StateMachine):

    INACTIVE = 0
    ACTIVE = 1
    SURPRESSED = 2

    def __init__(self, system, name):
        super(Behaviour, self).__init__(system, name)
        self._subsumed_behaviour = None
        self._status = Behaviour.INACTIVE
        self._previous_status = None
        self._name = name


    def subsume(self, subsumed_behaviour):
        self._subsumed_behaviour = subsumed_behaviour


    def activate(self):
        logger.info('Activing %s', self)
        if self._subsumed_behaviour:
            self._subsumed_behaviour.surpress()
        self._status = Behaviour.ACTIVE
        self.reset()


    def deactivate(self):
        logger.info('Deactiving %s', self)
        if self._subsumed_behaviour:
            self._subsumed_behaviour.unsurpress()
        self._status = Behaviour.INACTIVE
        self.reset()


    def surpress(self):
        logger.debug('Surpressing %s', self)
        self._previous_status = self._status
        self._status = Behaviour.SURPRESSED
        if self._subsumed_behaviour:
            self._subsumed_behaviour.surpress()


    def unsurpress(self):
        logger.debug('Unsurpressing %s', self)
        self._status = self._previous_status
        if self._subsumed_behaviour and self._status != Behaviour.ACTIVE:
            self._subsumed_behaviour.unsurpress()


    @property
    def active(self):
        return self._status == Behaviour.ACTIVE


    def __str__(self):
        return '<Behaviour: {0}>'.format(self._name)


    def update(self, now):
        if self.active:
            super(Behaviour, self).update(now)


    # def event_occurred(self, event):
    #     super(Behaviour, self).event_occurred(event)
