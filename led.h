/** @file led.h
 * @author Leonardo Bolstad
 * @date 04 Oct 2023
 * @brief variables regarding lights and modes
 */
#ifndef LED_H
#define LED_H

// FastLED
#define FASTLED_INTERNAL // Removes pragma message on compile
#include "FastLED.h"
#include <stdbool.h>

#define LEDS_PER_STRIP 30
#define NUM_STRIPS 4
#define VERT_STRIP_PIN 9
#define RSLANT_STRIP_PIN 10
#define HORIZ_STRIP_PIN 11
#define LSLANT_STRIP_PIN A2

#define VERT_STRIP 0 // Index in the led array
#define RSLANT_STRIP 1
#define HORIZ_STRIP 2
#define LSLANT_STRIP 3

#define DEFAULT_INTENSITY 255 // max 255
#define DEFAULT_SATURATION 255
#define LED_OFF CHSV(0, 0, 0)

CRGB leds[NUM_STRIPS][LEDS_PER_STRIP];
bool step_hue = true;
int current_hue = 10;
int shuffle = false;

// Initialize FASTLED controller
void led_setup(void);
static uint8_t active_hue = 0;
void pulse_effect(void);
void led_off(void);
#endif