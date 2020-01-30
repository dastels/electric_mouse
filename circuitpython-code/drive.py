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
Drive motor abstraction
"""

import board
from digitalio import DigitalInOut, Direction
from pulseio import PWMOut
from adafruit_motor.motor import DCMotor

import adafruit_logging as logging
logger = logging.getLogger('mouse')

STOPPED = 0
FORWARD = 1
REVERSE = 2
PIVOT_LEFT = 3
PIVOT_RIGHT = 4
VEER_LEFT = 5
VEER_RIGHT = 6

class AbstractDriveSystem(object):

    def __init__(self):
        self._state = STOPPED


    @property
    def state(self):
        return self._state


    def stop(self):
        self._state = STOPPED


    def go_forward(self):
        self._state = FORWARD


    def go_backward(self):
        self._state = REVERSE


    def pivot_left(self):
        self._state = PIVOT_LEFT


    def pivot_right(self):
        self._state = PIVOT_RIGHT


    def veer_left(self):
        self._state = VEER_LEFT


    def veer_right(self):
        self._state = VEER_RIGHT



class FakeDriveSystem(AbstractDriveSystem):

    def __init__(self):
        super(FakeDriveSystem, self).__init__()

    def enable(self):
        logger.info('Drive: Enabling motors')


    def disable(self):
        logger.info('Drive: Disabling motors')


    def stop(self):
        super(FakeDriveSystem, self).stop()
        logger.info('Drive: Stop')


    def go_forward(self):
        super(FakeDriveSystem, self).go_forward()
        logger.info('Drive: Forward')


    def go_backward(self):
        super(FakeDriveSystem, self).go_backward()
        logger.info('Drive: Reverse')


    def pivot_left(self):
        super(FakeDriveSystem, self).pivot_left()
        logger.info('Drive: Pivot left')


    def pivot_right(self):
        super(FakeDriveSystem, self).pivot_right()
        logger.info('Drive: Pivot right')


    def veer_left(self):
        super(FakeDriveSystem, self).veer_left()
        logger.info('Drive: Veer left')


    def veer_right(self):
        super(FakeDriveSystem, self).veer_right()
        logger.info('Drive: Veer right')




class DriveSystem(AbstractDriveSystem):

    def __init__(self):
        super(DriveSystem, self).__init__()
        self._motor_enable = DigitalInOut(board.A2)
        self._motor_enable.direction = Direction.OUTPUT
        self._motor_enable.value = False

        self._right_motor = DCMotor(PWMOut(board.D12), PWMOut(board.D13))
        self._left_motor = DCMotor(PWMOut(board.D4), PWMOut(board.D11))

        self._base_speed = 0.5


    @property
    def base_speed(self):
        return self._base_speed

    @base_speed.setter
    def base_speed(self, value):
        self._base_speed = value


    def enable(self):
        self._motor_enable.value = True


    def disable(self):
        self._motor_enable.value = False


    def stop(self):
        super(DriveSystem, self).stop()
        self._left_motor.throttle = 0
        self._right_motor.throttle = 0


    def go_forward(self):
        super(DriveSystem, self).go_forward()
        self._left_motor.throttle = self._base_speed
        self._right_motor.throttle = self._base_speed


    def go_backward(self):
        super(DriveSystem, self).go_backward()
        self._left_motor.throttle = -self._base_speed
        self._right_motor.throttle = -self._base_speed


    def pivot_left(self):
        super(DriveSystem, self).pivot_left()
        self._left_motor.throttle = -0.5 * self._base_speed
        self._right_motor.throttle = 0.5 * self._base_speed


    def pivot_right(self):
        super(DriveSystem, self).pivot_right()
        self._left_motor.throttle = 0.5 * self._base_speed
        self._right_motor.throttle = -0.5 * self._base_speed


    def veer_left(self):
        super(DriveSystem, self).veer_left()
        self._left_motor.throttle = 0.5 * self._base_speed
        self._right_motor.throttle = self._base_speed


    def veer_right(self):
        super(DriveSystem, self).veer_right()
        self._left_motor.throttle = self._base_speed
        self._right_motor.throttle = 0.5 * self._base_speed



def make_drive(debug):
    if debug:
        return FakeDriveSystem()
    else:
        return DriveSystem()
