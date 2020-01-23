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
Base state
"""

class State(object):

    def __init__(self, machine, name=''):
        self._machine = machine
        self._drive = machine.system.drive
        self._name = name


    @property
    def name(self):
        return self._name


    def enter(self, data=None):
        pass


    def exit(self, data=None):
        pass


    def update(self):
        pass


    def event_occurred(self, event, machine):
        pass


    def go_to(self, state_name, data=None):
        self._machine.go_to_state(state_name, data)


    def __str__(self):
        return '<State {0}:{1}>'.format(self._machine.name, self._name)
