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
Follow a heat source
"""

#pylint:disable=unused-argument

from behaviour import Behaviour
from state import State
from drive import STOPPED, FORWARD

import adafruit_logging as logging
logger = logging.getLogger('mouse')

class IdleState(State):

    def __init__(self, machine):
        super(IdleState, self).__init__(machine, 'idle')


    def event_occurred(self, event, machine):
        super(IdleState, self).event_occurred(event, machine)
        if event['type'] == 'ir' and event['where'] == 'hotspot':
            if event['what'] == 'present':
                self._machine.activate()
                self.go_to('track', event)


class TrackState(State):

    def __init__(self, machine):
        super(TrackState, self).__init__(machine, 'track')
        self._drive = machine.system.drive


    def event_occurred(self, event, machine):
        super(TrackState, self).event_occurred(event, machine)
        if event['type'] == 'ir':
            logger.info('Event in %s: %s', self, str(event))
            if event['where'] == 'hotspot' and event['what'] == 'absent':
                self._machine.deactivate()
            elif event['what'] == 'focus':
                if event['where'] == 'left':
                    self.go_to('veer_left')
                elif event['where'] == 'right':
                    self.go_to('veer_right')
                elif event['where'] == 'center':
                    self._machine.deactivate()



class VeerLeftState(State):

    def __init__(self, machine):
        super(VeerLeftState, self).__init__(machine, 'veer_left')
        self._drive = machine.system.drive


    def enter(self, data=None):
        super(VeerLeftState, self).enter(data)
        if self._drive.state == FORWARD:
            self._drive.veer_left()
        elif self._drive.state == STOPPED:
            self._drive.pivot_left()


    def event_occurred(self, event, machine):
        super(VeerLeftState, self).event_occurred(event, machine)
        if event['type'] == 'ir':
            logger.info('Event in %s: %s', self, str(event))
            if event['where'] == 'hotspot' and event['what'] == 'absent':
                self._machine.deactivate()
            elif event['what'] == 'focus':
                if event['where'] == 'left':
                    self.go_to('veer_left')
                elif event['where'] == 'right':
                    self.go_to('veer_right')
                elif event['where'] == 'center' or event['where'] == 'none':
                    self._machine.deactivate()



class VeerRightState(State):

    def __init__(self, machine):
        super(VeerRightState, self).__init__(machine, 'veer_right')
        self._drive = machine.system.drive
        self._side = None


    def enter(self, data=None):
        super(VeerRightState, self).enter(data)
        if self._drive.state == FORWARD:
            self._drive.veer_right()
        elif self._drive.state == STOPPED:
            self._drive.pivot_right()


    def event_occurred(self, event, machine):
        super(VeerRightState, self).event_occurred(event, machine)
        if event['type'] == 'ir':
            logger.info('Event in %s: %s', self, str(event))
            if event['where'] == 'hotspot' and event['what'] == 'absent':
                self._machine.deactivate()
            elif event['what'] == 'focus':
                if event['where'] == 'left':
                    self.go_to('veer_left')
                elif event['where'] == 'right':
                    self.go_to('veer_right')
                elif event['where'] == 'center' or event['where'] == 'none':
                    self._machine.deactivate()



class ChaseBehaviour(Behaviour):

    def __init__(self, system):
        super(ChaseBehaviour, self).__init__(system, 'chase')
        self.add_state(IdleState(self))
        self.add_state(TrackState(self))
        self.add_state(VeerLeftState(self))
        self.add_state(VeerRightState(self))
        self._initial_state = 'idle'
        self.reset()
