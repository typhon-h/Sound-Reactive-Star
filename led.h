#ifndef LED_H
#define LED_H

// FastLED
#define FASTLED_INTERNAL // Removes pragma message on compile
#include "FastLED.h"

#define LEDS_PER_STRIP 30
#define NUM_STRIPS 4
#define VERT_STRIP_PIN 6 // TODO: set pin
#define RSLANT_STRIP_PIN 2
#define HORIZ_STRIP_PIN 3
#define LSLANT_STRIP_PIN 4

#define VERT_STRIP 0
#define RSLANT_STRIP 1
#define HORIZ_STRIP 2
#define LSLANT_STRIP 3

#define DEFAULT_INTENSITY 255
#define DEFAULT_SATURATION 255
#define LED_OFF CHSV(0, 0, 0)

CRGB leds[NUM_STRIPS][LEDS_PER_STRIP];

// Initialize FASTLED controller
void led_setup(void);
static uint8_t active_hue = 0;
void pulse_effect(void);
#endif