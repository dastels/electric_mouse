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
Main electric mouse code
"""

import config
import colours

# imports
import time
from system import System
from behaviours import Behaviours
from forward_behaviour import ForwardBehaviour
from whisker_behaviour import WhiskerBehaviour
# from chase_behaviour import ChaseBehaviour
# from run_away_behaviour import RunAwayBehaviour
from wander_behaviour import WanderBehaviour
import drive

import adafruit_logging as logging
logger = logging.getLogger('mouse')

logger.setLevel(config.LOGGING_LEVEL)


# Figure out if we're autonomous
autonomous = False
try:
    with open(config.LOG_FILE, 'w') as fp:     # first, get rid of any previous log
        fp.write('Log\r\n')
    autonomous = True
except OSError as e:
    pass


# fake drive system if config.DEBUG is set and we're teathered.
system = System(drive.make_drive(config.DEBUG and not autonomous))
system.ir.threshold = 25
system.power_on()

#set the indicator and log to a file
if autonomous:
    system.indicate(colours.RED)
    import file_handler
    logger.addHandler(file_handler.FileHandler(config.LOG_FILE))
else:
    system.indicate(colours.GREEN)


# Setup behaviours

behaviours = Behaviours()
behaviours.add(ForwardBehaviour(system))
behaviours.add(WanderBehaviour(system))
# behaviours.add(ChaseBehaviour(system))
# behaviours.add(RunAwayBehaviour(system))
behaviours.add(WhiskerBehaviour(system))

# Timing control

update_time = 0
update_interval = 0.1

heartbeat_time = 0
heartbeat_interval = 1.0


# Start it up

ir = system.ir
drive = system.drive
left_whisker = system.left_whisker
right_whisker = system.right_whisker

drive.base_speed = config.BASE_SPEED
behaviours.start()

# Main loop
last = 0
while True:
    # update things
    now = time.monotonic()
    logger.debug('Update duration: %f', now - last)
    last = now
    system.update(now)

    behaviours.update(now)

    # Heartbeat
    if now > heartbeat_time:
        heartbeat_time = now + heartbeat_interval
        behaviours.event_occurred({'type': 'heartbeat'})

    # Whiskers
    if left_whisker.fell:
        behaviours.event_occurred({'type': 'whisker', 'where': 'left', 'what': 'fell'})
    if left_whisker.rose:
        behaviours.event_occurred({'type': 'whisker', 'where': 'left', 'what': 'rose'})
    if right_whisker.fell:
        behaviours.event_occurred({'type': 'whisker', 'where': 'right', 'what': 'fell'})
    if right_whisker.rose:
        behaviours.event_occurred({'type': 'whisker', 'where': 'right', 'what': 'rose'})

    # # Something is in my face
    # if system.in_your_face.rose:
    #     behaviours.event_occurred({'type': 'ir', 'where': 'in_your_face', 'what': 'present'})
    # elif system.in_your_face.fell:
    #     behaviours.event_occurred({'type': 'ir', 'where': 'in_your_face', 'what': 'absent'})

    # # Hot
    # if system.hotspot.rose:
    #     behaviours.event_occurred({'type': 'ir', 'where': 'hotspot', 'what': 'present'})
    # elif system.hotspot.fell:
    #     behaviours.event_occurred({'type': 'ir', 'where': 'hotspot', 'what': 'absent'})

    # focus = ir.focus
    # if focus == -1:
    #     behaviours.event_occurred({'type': 'ir', 'what': 'focus', 'where': 'left'})
    # elif focus == 0:
    #     behaviours.event_occurred({'type': 'ir', 'what': 'focus', 'where': 'center'})
    # elif focus == 1:
    #     behaviours.event_occurred({'type': 'ir', 'what': 'focus', 'where': 'right'})
    # else:
    #     behaviours.event_occurred({'type': 'ir', 'what': 'focus', 'where': 'none'})
