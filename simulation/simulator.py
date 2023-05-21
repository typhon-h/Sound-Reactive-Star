import serial
import sys
import time
import pygame
from pygame.locals import *
from leds import *

port = sys.argv[1]

pygame.init()
pygame.display.set_caption("LED Simulator")
surface = pygame.display.set_mode((795, 795))
clock = pygame.time.Clock()

arduino = serial.Serial(port=port,
                        baudrate=115200, timeout=.1)


strip_init(surface)

RUN_SIMULATION = True
while RUN_SIMULATION:
    to_update = []
    for event in pygame.event.get():
        if event.type == QUIT:
            RUN_SIMULATION = False

    try:
        data = arduino.readline()
        if len(data) == 0:
            print("Serial Empty")
            continue
        to_update += led_update(data.decode("utf-8").split(",")[:-1])
    except serial.serialutil.SerialException:
        print("Serial Error: Device not configured")
        try:
            time.sleep(2)
            arduino = serial.Serial(port=port,
                                    baudrate=115200, timeout=.1)
            print("Serial Reconnected: Reading Data...")
        except serial.serialutil.SerialException:
            pass

    clock.tick(60)
    print(f"FPS: {clock.get_fps():.2f}", end='\r')

    pygame.display.update(to_update)

pygame.quit()
