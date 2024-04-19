import pygame
import random
import sys
import serial
import time

# Initialize Pygame
pygame.init()

port = "/dev/cu.usbmodem112201" #sys.argv[1]



# Set display dimensions
WIDTH = 800
HEIGHT = 600

# Set colors
WHITE = (255, 255, 255)
BLACK = (0, 0, 0)
RED = (255, 0, 0)
GREEN = (0, 255, 0)
BLUE = (0, 0, 255)

# Set FPS (frames per second)
FPS = 60

# Create the screen
screen = pygame.display.set_mode((WIDTH, HEIGHT))
pygame.display.set_caption("Bar Graph")
    

# Function to draw smoothed bars
def draw_smoothed_bars(bar_values, smoothed_values):
    gap_width = 5
    bar_width = (WIDTH - (len(bar_values) - 1) * gap_width) // len(bar_values)
    for i in range(len(bar_values)):
        bar_height = (int(smoothed_values[i])) * 5
        pygame.draw.rect(screen, WHITE, (i * (bar_width + gap_width), HEIGHT - bar_height, bar_width, bar_height))

# Smooth the values
def smooth_values(old_values, new_values, smooth_factor):
    smoothed_values = []
    for old_val, new_val in zip(old_values, new_values):
        smoothed_values.append(old_val * smooth_factor + new_val * (1 - smooth_factor))
    return smoothed_values

# Main loop
def main():
    arduino = serial.Serial(port=port,
                        baudrate=115200, timeout=.1)
    # List of integers within a range
    num_values = 32
    smooth_factor = 0.7
    smoothed_values = [0 for i in range(num_values)]
    clock = pygame.time.Clock()

    running = True
    while running:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                running = False
                
        try:
            data = arduino.readline()
            if len(data) == 0:
                print("Serial Empty")
                continue
            bar_values = [int(i) for i in data.decode("utf-8").split(",")[:-1]]
            smoothed_values = smooth_values(smoothed_values, bar_values, smooth_factor)
            print(bar_values)
        except serial.serialutil.SerialException:
            print("Serial Error: Device not configured")
            try:
                time.sleep(2)
                arduino = serial.Serial(port=port,
                                        baudrate=115200, timeout=.1)
                print("Serial Reconnected: Reading Data...")
            except serial.serialutil.SerialException:
                pass              

        screen.fill(BLACK)
        draw_smoothed_bars(bar_values, smoothed_values)
        pygame.display.flip()

        # Update 60 times per second
        clock.tick(FPS)
    pygame.quit()
    sys.exit()

if __name__ == "__main__":
    main()
