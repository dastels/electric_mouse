import board
import digitalio
import storage

switch = digitalio.DigitalInOut(board.D9)
switch.direction = digitalio.Direction.INPUT
switch.pull = digitalio.Pull.UP

print('D9: {0}'.format(switch.value))
# If the switch is pressed CircuitPython can write to the drive
storage.remount('/', switch.value)
