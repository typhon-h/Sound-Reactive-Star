import os
import errno
import threading
import serial
import time
import tkinter as tk
from leds import *

RUN_SIMULATION = False

root = tk.Tk()
root.geometry("700x800")
root.title("LED Simulator")
START = tk.Button(root, text="✓", bg=RGB_to_STR((92, 219, 92)))


def simulate():
    START.grid_forget()
    root.update()
    RUN_SIMULATION = True
    arduino = serial.Serial(port='/dev/cu.usbmodem14201',
                            baudrate=115200, timeout=.1)

    while RUN_SIMULATION:
        try:
            data = arduino.readline().decode("utf-8").strip()
            if len(data) == 0:
                print("Serial Empty")
                continue
            led_update(data)
        except serial.serialutil.SerialException:
            print("Serial Error: Device not configured")
            try:
                time.sleep(2)
                arduino = serial.Serial(port='/dev/cu.usbmodem14201',
                                        baudrate=115200, timeout=.1)
                print("Serial Reconnected: Reading Data...")
            except serial.serialutil.SerialException:
                pass
    print("Terminated")


strip_init(root)

thread = threading.Thread(target=simulate, daemon=True)
START.config(command=thread.start)
START.grid(row=STRIP_LEN, column=STRIP_LEN)

tk.mainloop()
