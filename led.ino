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

void avg_freq_band_values(int *strips)
{
    for (int i = NOISE_INDEX + 1; i <= LOW_INDEX; i++)
    {
        strips[0] += fht_log_out[i] / (LOW_INDEX - NOISE_INDEX);
    }

    for (int i = LOW_INDEX + 1; i <= LOW_MID_INDEX; i++)
    {
        strips[1] += fht_log_out[i] / (LOW_MID_INDEX - LOW_INDEX);
    }

    for (int i = LOW_MID_INDEX + 1; i <= HIGH_MID_INDEX; i++)
    {
        strips[2] += fht_log_out[i] / (HIGH_MID_INDEX - LOW_MID_INDEX);
    }

    for (int i = HIGH_MID_INDEX + 1; i <= HIGH_INDEX; i++)
    {
        strips[3] += fht_log_out[i] / (HIGH_INDEX - HIGH_MID_INDEX);
    }

    return strips;
}

void pulse_effect() /// led update
{
    int strips[4] = {0, 0, 0, 0};
    avg_freq_band_values(strips);
    // for (int i = 0; i < 4; i++)
    // {
    //     Serial.print(strips[i]);
    //     Serial.print(",");
    // }
    // Serial.println();

    // -------------------------------
    // Turn on LEDS
    // max and min decide the range that the mic's range, based on the input it makes a
    // percentage / max and then outputs a percentage / 30 which decides how many lights are on
    int boundaries[4][2] = {
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
        // upper boynd of the value's target range
        byte hue;

        if (shuffle)
        {
            hue = random(0, 255);
        }
        else
        {
            hue = current_hue;
        }

        for (int i = 0; i < val; i++)
        {

            if (step_hue)
            {
                hue += 10;
            }
            leds[k][i] = CHSV(hue % 255, DEFAULT_SATURATION, DEFAULT_INTENSITY);
        }

        for (int i = val; i < LEDS_PER_STRIP; i++) // base state on
        {
            leds[k][i] = CRGB::Black;
        }
    }

    if (IrReceiver.isIdle())
    {
        FastLED.show();
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