import pygame
from pygame.locals import *
import random

STRIP_LEN = 30
ROWS = COLS = STRIP_LEN * 2 + 1
CELL_SIZE = 13
PADDING = 1

OFF_COLOR = (0, 0, 0)
BACKGROUND = (80, 80, 80)

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

surface = None


def RGB_to_STR(color):
    r = color[0] if color[0] < 255 else 255
    g = color[1] if color[1] < 255 else 255
    b = color[2] if color[2] < 255 else 255
    return '#%02x%02x%02x' % (r, g, b)


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
        label = Rect(row*CELL_SIZE, col*CELL_SIZE, CELL_SIZE, CELL_SIZE)
        target.append(label)


def strip_init(s):
    global surface
    surface = s
    surface.fill(BACKGROUND)
    for row in range(ROWS):
        for col in range(COLS):
            add_to_strip(row, col)

    for l in range(NUM_STRIPS):
        for led in range(STRIP_LEN):
            set_led(l, led, OFF_COLOR)
    pygame.display.flip()


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

    curr_color = surface.get_at(
        (strip_to_modify[index][LED][0].left, strip_to_modify[index][LED][0].top))

    if (curr_color.r, curr_color.g, curr_color.b) != color:
        for led in strip_to_modify[index][LED]:
            pygame.draw.rect(surface, color, led)
        return strip_to_modify[index][LED]
    return False


def reset():
    for strip in range(NUM_STRIPS):
        for led in range(STRIP_LEN):
            set_led(strip, led, OFF_COLOR)


def led_update(data):
    updated = []
    for strip in range(NUM_STRIPS):
        for led in range(STRIP_LEN):
            index = 3 * (strip * STRIP_LEN + led)
            try:
                if pixels := set_led(strip, led, (int(data[index])*8,
                                                  int(data[index+1])*8, int(data[index+2])*8)):
                    updated += pixels
            except (IndexError, ValueError):
                pass
    return updated
