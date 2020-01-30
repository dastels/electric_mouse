import board
import digitalio
import storage

switch = digitalio.DigitalInOut(board.D9)
switch.direction = digitalio.Direction.INPUT
switch.pull = digitalio.Pull.UP

# If the switch is presses CircuitPython can write to the drive
storage.remount('/', switch.value)
