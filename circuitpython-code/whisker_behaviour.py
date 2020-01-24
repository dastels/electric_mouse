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
Respond to a whisker bump
"""

#pylint:disable=unused-argument

import time
from behaviour import Behaviour
from state import State
import adafruit_logging as logging
logger = logging.getLogger('mouse')

class IdleState(State):

    def __init__(self, machine):
        super(IdleState, self).__init__(machine, 'idle')


    def enter(self, data=None):
        super(IdleState, self).enter(data)
        self._drive.stop()


    def event_occurred(self, event, machine):
        super(IdleState, self).event_occurred(event, machine)
        if event['type'] == 'whisker' and event['what'] == 'fell':
            self._machine.activate()
            self.go_to('backward', event)



class GoBackwardState(State):

    def __init__(self, machine, backup_time):
        super(GoBackwardState, self).__init__(machine, 'backward')
        self._backup_time = backup_time
        self._left_whisker = machine.system.left_whisker
        self._right_whisker = machine.system.right_whisker
        self._drive = machine.system.drive
        self._side = None
        self._started_reversing = 0


    def enter(self, data=None):
        super(GoBackwardState, self).enter(data)
        self._side = data['where']
        self._started_reversing = time.monotonic()
        self._drive.go_backward()


    def update(self, now):
        super(GoBackwardState, self).update(now)
        time_expired = now > self._started_reversing + self._backup_time
        whiskers_clear = self._left_whisker.value and self._right_whisker.value
        if whiskers_clear:
            logger.debug('Whisters clear in %s', self)
        if time_expired:
            logger.debug('Reverse timer expired in %s', self)
        if time_expired and whiskers_clear:
            self.go_to('pivot', self._side)



class PivotState(State):

    def __init__(self, machine, pivot_time):
        super(PivotState, self).__init__(machine, 'pivot')
        self._started_pivoting = 0
        self._pivot_time = pivot_time
        self._drive = machine.system.drive
        self._side = None


    def enter(self, data=None):
        super(PivotState, self).enter(data)
        self._started_pivoting = time.monotonic()
        if data == 'left':
            self._drive.pivot_right()
        else:
            self._drive.pivot_left()


    def update(self, now):
        super(PivotState, self).update(now)
        if now > self._started_pivoting + self._pivot_time:
            self._machine.deactivate()



class WhiskerBehaviour(Behaviour):

    def __init__(self, system, backup_time=0.25, pivot_time=0.25):
        super(WhiskerBehaviour, self).__init__(system, 'whisker')
        self.add_state(IdleState(self))
        self.add_state(GoBackwardState(self, backup_time))
        self.add_state(PivotState(self, pivot_time))
        self._initial_state = 'idle'
        self.reset()
