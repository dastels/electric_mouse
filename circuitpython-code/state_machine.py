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
The satet machine
"""

import adafruit_logging as logging
logger = logging.getLogger('mouse')

class StateMachine(object):

    def __init__(self, system, name):
        self._system = system
        self._name = name
        self._state = None
        self._states = {}
        self._initial_state = None


    def dispose(self):
        self._states.clear()


    def add_state(self, state):
        self._states[state.name] = state


    @property
    def initial_state(self):
        return self._initial_state


    def reset(self):
        self.go_to_state(self._initial_state)


    def go_to_state(self, state_name, data=None):
        if state_name:
            if self._state:
                logger.info('Exiting %s', self._state)
                self._state.exit(self)
            self._state = self._states[state_name]
            logger.info('Entering %s', self._state)
            self._state.enter(data)
        else:
            logger.critical('None state name')
            raise Exception('None state name')


    def update(self, now):
        if self._state:
 #           logger.debug('Updating %s', self._state)
            self._state.update(now)


    def event_occurred(self, event):
        if self._state:
            logger.debug('Event in %s: %s', self._state, str(event))
            self._state.event_occurred(event, self)


    @property
    def current_state(self):
        return self._state.name if self._state else 'NOT_SET'


    @property
    def name(self):
        return self._name

    @property
    def system(self):
        return self._system


    def __str__(self):
        return '<State Machine: {0}>'.format(self._name)
