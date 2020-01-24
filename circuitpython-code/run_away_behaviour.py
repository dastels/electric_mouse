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
Run Away

While it would be nice to be able to turn precisely 180 deg, it's more
important to get turned in more ot less away from where we were heading.
With encoders on the motors we could be more precise. Maybe in build 2.
"""

#pylint:disable=unused-argument

import time
from behaviour import Behaviour
from state import State
from drive import STOPPED, FORWARD, REVERSE, PIVOT_LEFT, PIVOT_RIGHT, VEER_LEFT, VEER_RIGHT

import adafruit_logging as logging
logger = logging.getLogger('mouse')


class RotateState(State):


    def __init__(self, machine, rotate_time):
        super(RotateState, self).__init__(machine, 'rotate')
        self._drive = machine.system.drive
        self._started_rotating = 0
        self._rotate_time = rotate_time


    def enter(self, data=None):
        super(RotateState, self).enter(data)
        self._started_rotating = time.monotonic()
        self._drive.pivot_right()


    def update(self, now):
        super(RotateState, self).update(now)
        time_expired = now > self._started_rotating + self._rotate_time
        if time_expired:
            self._machine.deactivate()



class RunAwayBehaviour(Behaviour):

    def __init__(self, system, rotate_time=0.25):
        super(RunAwayBehaviour, self).__init__(system, 'runaway')
        self.add_state(RotateState(self, rotate_time))
        self._initial_state = 'rotate'
