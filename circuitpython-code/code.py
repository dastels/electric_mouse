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
DEBUG = True


# imports
import time
from system import System
from behaviours import Behaviours
from forward_behaviour import ForwardBehaviour
from whisker_behaviour import WhiskerBehaviour
from chase_behaviour import ChaseBehaviour
import drive

import adafruit_logging as logging
logger = logging.getLogger('mouse')

logger.setLevel(logging.INFO)


system = System(drive.make_drive(DEBUG))
system.ir.threshold = 25

# Setup logging to file if possible
try:
    with open(settings.LOG_FILE, 'w') as fp:     # first, get rid of any previous log
        fp.write('Log\r\n')
    system.indicate(colours.RED)
    import file_handler
    logger.addHandler(file_handler.FileHandler(settings.LOG_FILE))
except OSError as e:
    system.indicate(colours.GREEN)
    print(e)
    print('Logging to serial')

# Setup behaviours

behaviours = Behaviours()
behaviours.add(ForwardBehaviour(system))
behaviours.add(ChaseBehaviour(system))
behaviours.add(WhiskerBehaviour(system))

# Timing control

update_time = 0
update_interval = 0.1

heartbeat_time = 0
heartbeat_interval = 1.0


# Start it up

system.power_on()
ir = system.ir
drive = system.drive
left_whisker = system.left_whisker
right_whisker = system.right_whisker

behaviours.start()

# Main loop

while True:
    # update things
    now = time.monotonic()
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

    # Hot
    if system.hotspot.rose:
        behaviours.event_occurred({'type': 'ir', 'where': 'hotspot', 'what': 'present'})
    elif system.hotspot.fell:
        behaviours.event_occurred({'type': 'ir', 'where': 'hotspot', 'what': 'absent'})

    focus = ir.focus
    if focus == -1:
        behaviours.event_occurred({'type': 'ir', 'what': 'focus', 'where': 'left'})
    elif focus == 0:
        behaviours.event_occurred({'type': 'ir', 'what': 'focus', 'where': 'center'})
    elif focus == 1:
        behaviours.event_occurred({'type': 'ir', 'what': 'focus', 'where': 'right'})
    else:
        behaviours.event_occurred({'type': 'ir', 'what': 'focus', 'where': 'none'})
