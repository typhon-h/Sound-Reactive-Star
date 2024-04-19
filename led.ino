/** @file led.ino
 * @author Leonardo Bolstad
 * @date 04 Oct 2023
 * @brief Code regarding lights and modes
 */
#include "microphone.h"
#include <FHT.h> // FFT-like library
#include "led.h"
#include "tasks.h"
#include "ir.h"
/**
 * @brief Initializes the strips within FastLED with default settings.
 *
 */
void led_setup()
{
    FastLED.addLeds<NEOPIXEL, VERT_STRIP_PIN>(leds[VERT_STRIP], LEDS_PER_STRIP);
    FastLED.addLeds<NEOPIXEL, RSLANT_STRIP_PIN>(leds[RSLANT_STRIP], LEDS_PER_STRIP);
    FastLED.addLeds<NEOPIXEL, HORIZ_STRIP_PIN>(leds[HORIZ_STRIP], LEDS_PER_STRIP);
    FastLED.addLeds<NEOPIXEL, LSLANT_STRIP_PIN>(leds[LSLANT_STRIP], LEDS_PER_STRIP);

    FastLED.setBrightness(DEFAULT_INTENSITY);
    FastLED.setCorrection(TypicalLEDStrip);

    // limit my draw to 2.7A at 5v of power draw
    FastLED.setMaxPowerInVoltsAndMilliamps(5, 2500);

    FastLED.clear();
    FastLED.show();
}

void led_update()
{
    switch (active_effect)
    {
    case PULSE: // Fallthrough
    default:
        pulse_effect();
    }

    if (IrReceiver.isIdle())
    {
        FastLED.show();
    }
}

void pulse_effect()
{
    int strips[4] = {0, 0, 0, 0};
    avg_freq_band_values(strips);

    // -------------------------------
    // Turn on LEDS
    // max and min decide the range that the mic's range, based on the input it makes a
    // percentage / max and then outputs a percentage / 30 which decides how many lights are on
    int boundaries[4][2] = {
        // TODO: Find a better way of controlling this universally
        {19, 90},
        {10, 90},
        {5, 90},
        {2, 90},
    };

    for (int k = 0; k < NUM_STRIPS; k++)
    {
        if (strips[k] > boundaries[k][1])
            strips[k] = boundaries[k][1];

        if (strips[k] < boundaries[k][0])
            strips[k] = boundaries[k][0];

        byte val = map(strips[k], boundaries[k][0], boundaries[k][1], 0, LEDS_PER_STRIP);

        for (int i = 0; i < val; i++)
        {
            leds[k][i] = CHSV(current_hue % 255, DEFAULT_SATURATION, DEFAULT_INTENSITY);
        }

        for (int i = val; i < LEDS_PER_STRIP; i++) // base state off
        {
            leds[k][i] = CRGB::Black;
        }
    }
}

void led_off()
{ // LED OFF

    for (int k = 0; k < NUM_STRIPS; k++)
    {
        for (int i = 0; i < LEDS_PER_STRIP; i++) // base state on
        {
            leds[k][i] = CRGB::Black;
        }
    }
    FastLED.show();
}