#!/usr/bin/env python3
import struct
import time
import keyboard
from getpass import getpass
from bluepy.btle import Peripheral, DefaultDelegate

parser = argparse.ArgumentParser(description='Print advertisement data from a BLE device')
parser.add_argument('addr', metavar='A', type=str, help='Address of the form XX:XX:XX:XX:XX:XX')
args = parser.parse_args()
addr = args.addr.lower()
if len(addr) != 17:
    raise ValueError("Invalid address supplied")

SERVICE_UUID = "4607eda0-f65e-4d59-a9ff-84420d87a4ca"
CHAR_UUIDS = None # TODO: add your characteristics
CMD_UUID = "4607eda1-f65e-4d59-a9ff-84420d87a4ca"

class RobotController():

    def __init__(self, address):

        self.robot = Peripheral(addr)
        print("connected")

        # keep state for keypresses
        self.pressed = {"up": False, "down": False, "right": False, "left": False}
        # TODO get service from robot
        # TODO get characteristic handles from service/robot
        # TODO enable notifications if using notifications

        sv = buckler.getServiceByUUID(SERVICE_UUID)
        # Get characteristic
        ch = sv.getCharacteristics(CMD_UUID)[0]

        keyboard.hook(self.on_key_event)

    def on_key_event(self, event):
        # print key name
        print(event.name)
        # if a key unrelated to direction keys is pressed, ignore
        if event.event_type == keyboard.KEY_DOWN:
            # if that key is already pressed down, ignore
            if self.pressed[event.name]: return
            # set state of key to pressed
            self.pressed[event.name] = True
            # TODO write to characteristic to change direction
            cmd.write(bytes(1))
        elif event.event_type == keyboard.KEY_UP:
            if self.pressed[event.name]: return
            # set state of key to pressed
            self.pressed[event.name] = True
            # TODO write to characteristic to change direction
            cmd.write(bytes(1))
        elif event.event_type == keyboard.KEY_LEFT:
            if self.pressed[event.name]: return
            # set state of key to pressed
            self.pressed[event.name] = True
            # TODO write to characteristic to change direction
            cmd.write(bytes(1))
        elif event.event_type == keyboard.KEY_RIGHT:
            if self.pressed[event.name]: return
            # set state of key to pressed
            self.pressed[event.name] = True
            # TODO write to characteristic to change direction
            cmd.write(bytes(1))
        else:
            # set state of key to released
            self.pressed[event.name] = False
            # TODO write to characteristic to stop moving in this direction
            cmd.write(bytes(1))
    def __enter__(self):
        return self
    def __exit__(self, exc_type, exc_value, traceback):
        self.robot.disconnect()

with RobotController(addr) as robot:
    getpass('Use arrow keys to control robot')
