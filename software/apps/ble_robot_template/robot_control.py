#!/usr/bin/env python3
import struct
import time
import keyboard
from getpass import getpass
from bluepy.btle import Peripheral, DefaultDelegate
import argparse

parser = argparse.ArgumentParser(description='Print advertisement data from a BLE device')
addr = "C0:98:E5:49:00:09"
addr = addr.lower()
if len(addr) != 17:
    raise ValueError("Invalid address supplied")

SERVICE_UUID = "4607eda0-f65e-4d59-a9ff-84420d87a4ca"
CHAR_UUIDS = None # TODO: add your characteristics
FORWARD_UUID = "4607eda1-f65e-4d59-a9ff-84420d87a4ca"
BACK_UUID = "4607eda2-f65e-4d59-a9ff-84420d87a4ca"
LEFT_UUID = "4607eda3-f65e-4d59-a9ff-84420d87a4ca"
RIGHT_UUID = "4607eda4-f65e-4d59-a9ff-84420d87a4ca"
STOP_UUID = "4607eda5-f65e-4d59-a9ff-84420d87a4ca"

class RobotController():

    def __init__(self):

        self.robot = Peripheral(addr)
        print("connected")

        # keep state for keypresses
        self.pressed = {"up": False, "down": False, "right": False, "left": False}
        # TODO get service from robot
        # TODO get characteristic handles from service/robot
        # TODO enable notifications if using notifications
        self.sv = self.robot.getServiceByUUID(SERVICE_UUID)

        self.forward = self.sv.getCharacteristics(FORWARD_UUID)[0]
        self.back = self.sv.getCharacteristics(BACK_UUID)[0]
        self.left = self.sv.getCharacteristics(LEFT_UUID)[0]
        self.right = self.sv.getCharacteristics(RIGHT_UUID)[0]
        self.stop = self.sv.getCharacteristics(STOP_UUID)[0]

        keyboard.hook(self.on_key_event)

    def on_key_event(self, event):
        # print key name
        print(event.name)
        # if a key unrelated to direction keys is pressed, ignore
        if event.name not in self.pressed: return
        # if a key is pressed down
        if event.event_type == keyboard.KEY_DOWN:
            # if that key is already pressed down, ignore
            if self.pressed[event.name]: return
            # set state of key to pressed
            self.pressed[event.name] = True
            # TODO write to characteristic to change direction
            if event.name == "up":
                self.forward.write(bytes(True))
            elif event.name == "down":
                self.back.write(bytes(True))
            elif event.name == "left":
                self.left.write(bytes(True))
            elif event.name == "right":
                self.right.write(bytes(True))
        else:
            # set state of key to released
            self.pressed[event.name] = False
            # TODO write to characteristic to stop moving in this direction
            self.stop.write(bytes(True))
    def __enter__(self):
        return self
    def __exit__(self, exc_type, exc_value, traceback):
        self.robot.disconnect()

with RobotController() as robot:
    getpass('Use arrow keys to control robot')
