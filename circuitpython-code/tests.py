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
Tests
"""
#pylint:disable=redefined-outer-name

from state_machine import StateMachine
from state import State

import adafruit_logging as logging

logger = logging.getLogger('mouse')
logger.setLevel(logging.DEBUG)


class State1(State):

    def __init__(self, machine):
        super(State1, self).__init__(machine, 'one')


    def update(self, data=None):
        self.go_to('two')


    def event_occurred(self, event, machine):
        if event == 1:
            self.go_to('three')
        elif event == 2:
            self.go_to('two')
        elif event == 3:
            self.go_to('one')




class State2(State):


    def __init__(self, machine):
        super(State2, self).__init__(machine, 'two')


    def update(self, data=None):
        self.go_to('three')


    def event_occurred(self, event, machine):
        if event == 1:
            pass
        elif event == 2:
            pass
        elif event == 3:
            self.go_to('three')


class State3(State):

    def __init__(self, machine):
        super(State3, self).__init__(machine, 'three')


    def update(self, data=None):
        self.go_to('one')


    def event_occurred(self, event, machine):
        if event == 2:
            self.go_to('two')




class Machine(StateMachine):

    def __init__(self, system, subsumed_machine=None):
        super(Machine, self).__init__(system, 'test-machine', subsumed_machine)
        self.add_state(State1(self))
        self.add_state(State2(self))
        self.add_state(State3(self))
        self._initial_state = 'one'


def assert_equal(first, second, msg):
    """The default assertEqual implementation, not type specific."""
    if not first == second:
        errorMessage = '%s: %s != %s' % (msg, first, second)
        raise AssertionError(errorMessage)


class FakeSystem(object):
    @property
    def drive(self):
        return None

machine = Machine(FakeSystem())
assert_equal(machine.current_state, 'NOT_SET', 'wrong instantiated state')

machine.activate()
assert_equal(machine.current_state, 'one', 'wrong initial state')

machine.update()
assert_equal(machine.current_state, 'two', 'wrong second state')

machine.update()
assert_equal(machine.current_state, 'three', 'wrong third state')

machine.update()
assert_equal(machine.current_state, 'one', 'wrong fourth state')

machine.deactivate()
assert_equal(machine.current_state, 'NOT_SET', 'wrong deactivated state')

machine.activate()
assert_equal(machine.current_state, 'one', 'wrong reactivated state')

machine.event_occurred(1)
assert_equal(machine.current_state, 'three', 'wrong one->1->three state')

machine.event_occurred(2)
assert_equal(machine.current_state, 'two', 'wrong one->1->three state')

machine.event_occurred(3)
assert_equal(machine.current_state, 'three', 'wrong one->1->three state')

print('All tests pass')
