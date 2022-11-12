import os
import errno
import threading
import serial
import time
import tkinter as tk
from leds import *

RUN_SIMULATION = False

root = tk.Tk()
root.geometry("685x630")
root.title("LED Simulator")
START = tk.Button(root, text="✓", bg=RGB_to_STR((92, 219, 92)))

arduino = serial.Serial(port='/dev/cu.usbserial-DA00SOS8',
                        baudrate=115200, timeout=.1)


def simulate():
    START.grid_forget()
    root.update()
    RUN_SIMULATION = True

    while RUN_SIMULATION:
        data = arduino.readline().decode("utf-8").strip()
        if len(data) == 0:
            print("Serial Empty")
            continue
        led_update(data)
    print("Terminated")


strip_init(root)

thread = threading.Thread(target=simulate, daemon=True)
START.config(command=thread.start)
START.grid(row=STRIP_LEN, column=STRIP_LEN)

tk.mainloop()
