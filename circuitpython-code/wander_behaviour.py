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
Wander about
"""

#pylint:disable=unused-argument

import time
from behaviour import Behaviour
from state import State
from drive import STOPPED, FORWARD
from random import random

import adafruit_logging as logging
logger = logging.getLogger('mouse')


class IdleState(State):

    def __init__(self, machine):
        super(IdleState, self).__init__(machine, 'idle')


    def event_occurred(self, event, machine):
        super(IdleState, self).event_occurred(event, machine)
        if event['type'] == 'heartbeat':
            self._machine.activate()
            chance = random()
            if chance < 0.25:
                self.go_to('veer_left')
            elif chance < 0.5:
                self.go_to('veer_right')
            else:
                self._machine.deactivate()


class VeerLeftState(State):

    def __init__(self, machine):
        super(VeerLeftState, self).__init__(machine, 'veer_left')
        self._drive = machine.system.drive
        self._timeout = 0


    def enter(self, data=None):
        super(VeerLeftState, self).enter(data)
        self._timeout = time.monotonic() + (random() * 0.25)
        if self._drive.state == FORWARD:
            self._drive.veer_left()
        elif self._drive.state == STOPPED:
            self._drive.pivot_left()


    def update(self, now):
        if now > self._timeout:
            self._machine.deactivate()


class VeerRightState(State):

    def __init__(self, machine):
        super(VeerRightState, self).__init__(machine, 'veer_right')
        self._drive = machine.system.drive
        self._timeout = 0


    def enter(self, data=None):
        super(VeerRightState, self).enter(data)
        self._timeout = time.monotonic() + (random() * 0.25)
        if self._drive.state == FORWARD:
            self._drive.veer_right()
        elif self._drive.state == STOPPED:
            self._drive.pivot_right()


    def update(self, now):
        if now > self._timeout:
            self._machine.deactivate()


class WanderBehaviour(Behaviour):

    def __init__(self, system):
        super(WanderBehaviour, self).__init__(system, 'wander')
        self.add_state(IdleState(self))
        self.add_state(VeerLeftState(self))
        self.add_state(VeerRightState(self))
        self._initial_state = 'idle'
        self.reset()
