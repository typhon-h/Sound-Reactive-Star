import os
import errno
import threading
import signal
import tkinter as tk
from leds import *

RUN_SIMULATION = False
PIPE = 'simpipe'

root = tk.Tk()
root.geometry("685x630")
root.title("LED Simulator")
START = tk.Button(root, text="✓", bg=RGB_to_STR((92, 219, 92)))

try:
    os.mkfifo(PIPE)
except OSError as oe:
    if oe.errno != errno.EEXIST:
        raise


def simulate():
    START.grid_forget()
    root.update()
    RUN_SIMULATION = True

    while RUN_SIMULATION:
        print("Opening Pipe")
        with open(PIPE) as pipe:
            print("Pipe Opened")
            data = os.read(pipe.fileno(), 1)
            if len(data) == 0:
                print("Nothing written to pipe")
                break
            led_update(data)
    print("Terminated")


strip_init(root)

thread = threading.Thread(target=simulate, daemon=True)
START.config(command=thread.start)
START.grid(row=STRIP_LEN, column=STRIP_LEN)

tk.mainloop()
