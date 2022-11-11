import tkinter as tk

STRIP_LEN = 30
ROWS = COLS = STRIP_LEN * 2 + 1
CELL_SIZE = 5
PADDING = 1

OFF_COLOR = (0, 0, 0)

LED = 0
COLOR = 1

NUM_STRIPS = 4
VERT = [[[], OFF_COLOR] for led in range(STRIP_LEN)]
RSLANT = [[[], OFF_COLOR] for led in range(STRIP_LEN)]
HORIZ = [[[], OFF_COLOR] for led in range(STRIP_LEN)]
LSLANT = [[[], OFF_COLOR] for led in range(STRIP_LEN)]

VERT_STRIP = 0
RSLANT_STRIP = 1
HORIZ_STRIP = 2
LSLANT_STRIP = 3

root = None


def RGB_to_STR(color):
    return '#%02x%02x%02x' % (color[0], color[1], color[2])


def add_to_strip(row, col):
    target = None
    # vert
    if col == STRIP_LEN:
        if row < STRIP_LEN:
            target = VERT[-(row+1)][LED]
        elif row > STRIP_LEN:
            target = VERT[row-STRIP_LEN-1][LED]

    # rslant
    if col+row == STRIP_LEN*2:
        if row < STRIP_LEN:
            target = RSLANT[-(row+1)][LED]
        elif row > STRIP_LEN:
            target = RSLANT[row-STRIP_LEN-1][LED]

    # horiz
    if row == STRIP_LEN:
        if col < STRIP_LEN:
            target = HORIZ[-(col+1)][LED]
        elif col > STRIP_LEN:
            target = HORIZ[col-STRIP_LEN-1][LED]

    # lslant
    if col == row and col != STRIP_LEN:
        if row < STRIP_LEN:
            target = LSLANT[-(row+1)][LED]
        elif row > STRIP_LEN:
            target = LSLANT[row-STRIP_LEN-1][LED]

    if target != None:
        label = tk.Label(root, text="   ", font=("Times 1"))
        label.grid(row=row, column=col, padx=PADDING, pady=PADDING)
        target.append(label)


def strip_init(rt):
    root = rt
    for row in range(ROWS):
        root.rowconfigure(row, minsize=CELL_SIZE)
        root.columnconfigure(row, minsize=CELL_SIZE)
        for col in range(COLS):
            add_to_strip(row, col)

    for l in range(NUM_STRIPS):
        for led in range(STRIP_LEN):
            set_led(l, led, OFF_COLOR)


def set_led(strip, index, color):
    if strip == VERT_STRIP:
        strip_to_modify = VERT
    elif strip == RSLANT_STRIP:
        strip_to_modify = RSLANT
    elif strip == HORIZ_STRIP:
        strip_to_modify = HORIZ
    elif strip == LSLANT_STRIP:
        strip_to_modify = LSLANT
    else:
        print("Error: Invalid Strip")
        return

    for led in strip_to_modify[index][LED]:
        led.config(bg=RGB_to_STR(color))


def reset():
    for strip in range(NUM_STRIPS):
        for led in range(STRIP_LEN):
            set_led(strip, led, OFF_COLOR)


def led_update(data):
    values = data.split(",")
    index = 0
    for strip in range(NUM_STRIPS):
        for led in range(STRIP_LEN):
            set_led(strip, led, (values[index],
                                 values[index+1], values[index+2]))
            index += 3

    root.update()
