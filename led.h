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

typedef enum
{
    PULSE,
    INVERSE_PULSE,
    COLOR_PULSE,
    BEAT,
    SPIRAL,
    NUM_EFFECTS
} EFFECT_T;

#define LEDS_PER_STRIP 30
#define NUM_STRIPS 4
#define VERT_STRIP_PIN 10
#define RSLANT_STRIP_PIN 9
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

static uint8_t current_hue = 0;
static EFFECT_T active_effect = COLOR_PULSE;

// Initialize FASTLED controller
void led_setup(void);
void led_update(void);
void led_off(void);
#endif