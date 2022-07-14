#ifndef LED_H
#define LED_H

// FastLED
#define FASTLED_INTERNAL // Removes pragma message on compile
#include "FastLED.h"

#define LED_PIN 1 // TODO: set pin

#define LED_PER_STRIP 30
#define NUM_STRIPS 8
#define LED_COUNT LED_PER_STRIP *NUM_STRIPS

#define UC_STRIP_INDEX LED_PER_STRIP * 0 // TODO: set index
#define UR_STRIP_INDEX LED_PER_STRIP * 1
#define CR_STRIP_INDEX LED_PER_STRIP * 2
#define DR_STRIP_INDEX LED_PER_STRIP * 3
#define DC_STRIP_INDEX LED_PER_STRIP * 4
#define DL_STRIP_INDEX LED_PER_STRIP * 5
#define CL_STRIP_INDEX LED_PER_STRIP * 6
#define UL_STRIP_INDEX LED_PER_STRIP * 7

#define DEFAULT_INTENSITY 255
#define DEFAULT_SATURATION 255
#define LED_OFF CHSV(0, 0, 0)

CRGB leds[LED_COUNT];

// Initialize FASTLED controller
void led_setup(void);
#endif