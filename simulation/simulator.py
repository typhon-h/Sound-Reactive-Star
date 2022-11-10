import tkinter as tk
from leds import *

RUN_SIMULATION = False


root = tk.Tk()
root.geometry("685x630")
root.title("LED Simulator")


def abort():
    START.config(text="✓", command=simulate, bg=RGB_to_STR((92, 219, 92)))
    RUN_SIMULATION = False


def simulate():
    START.config(text=' X ', command=abort, bg=RGB_to_STR((255, 0, 33)))
    RUN_SIMULATION = True


strip_init(root)
START = tk.Button(root, text="✓", command=simulate,
                  bg=RGB_to_STR((92, 219, 92)))
START.grid(row=STRIP_LEN, column=STRIP_LEN)

tk.mainloop()
