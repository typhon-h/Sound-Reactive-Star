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
    case BEAT:
        beat_effect();
        break;
    case SPIRAL:
        spiral_effect();
        break;
    case INVERSE_PULSE:
        pulse_effect(true);
        break;
    case COLOR_PULSE:
        color_effect();
        break;
    case PULSE: // Fallthrough
    default:
        pulse_effect(false);
    }

    if (IrReceiver.isIdle())
    {
        FastLED.show();
    }
}

void spiral_effect()
{
    int strips[4] = {0, 0, 0, 0};
    avg_freq_band_values(strips);

    int boundaries[4][2] = {
        {get_level_boundary(LOW_MIN_LOWER, LOW_MAX_LOWER), get_level_boundary(LOW_MIN_UPPER, LOW_MAX_UPPER)},
        {get_level_boundary(LOWMID_MIN_LOWER, LOWMID_MAX_LOWER), get_level_boundary(LOWMID_MIN_UPPER, LOWMID_MAX_UPPER)},
        {get_level_boundary(HIGHMID_MIN_LOWER, HIGHMID_MAX_LOWER), get_level_boundary(HIGHMID_MIN_UPPER, HIGHMID_MAX_UPPER)},
        {get_level_boundary(HIGH_MIN_LOWER, HIGH_MAX_LOWER), get_level_boundary(HIGH_MIN_UPPER, HIGH_MAX_UPPER)},

    };

    int thresholds[4] = {0};
    float scalar = 0.6f; // Position of the threshold in the range as percentage

    for (int i = 0; i < 4; i++)
    {
        thresholds[i] = boundaries[i][0] + (boundaries[i][1] - boundaries[i][0]) * scalar;
    }

    CRGB tmp[LEDS_PER_STRIP];
    memcpy(tmp, leds[0], sizeof(leds[0]));

    for (int i = 0; i < NUM_STRIPS - 1; i++)
    {
        for (int j = LEDS_PER_STRIP - 1; j > 0; j--)
        {
            leds[i][j] = leds[(i + 1) % NUM_STRIPS][j - 1];
        }
    }

    for (int j = LEDS_PER_STRIP - 1; j > 0; j--)
    {
        leds[NUM_STRIPS - 1][j] = tmp[j - 1];
    }

    for (int i = 0; i < NUM_STRIPS; i++)
    {
        leds[i][0] = strips[i] >= thresholds[i] ? CHSV((current_hue + 64 * i) % 255, DEFAULT_SATURATION, DEFAULT_INTENSITY) : CHSV(0, 0, 0);
    }
}

void beat_effect()
{
    // We only care about LOWMID (index 1)
    int strips[4] = {0, 0, 0, 0};
    avg_freq_band_values(strips);

    int upper = get_level_boundary(LOW_MIN_UPPER, LOW_MAX_UPPER);
    int lower = get_level_boundary(LOW_MIN_LOWER, LOW_MAX_LOWER);
    float scalar = 0.6f; // Position of the threshold in the range as percentage
    int threshold = lower + (upper - lower) * scalar;

    for (int i = RSLANT_STRIP; i < NUM_STRIPS; i++)
    { // all strips except vertical
        for (int j = LEDS_PER_STRIP - 1; j > 0; j--)
        {
            leds[i][j] = leds[i][j - 1];
        }
        leds[i][0] = leds[VERT_STRIP][0];
    }

    // // vertical
    for (int i = 0; i < LEDS_PER_STRIP - 1; i++)
    {
        leds[VERT_STRIP][i] = leds[VERT_STRIP][i + 1];
    }

    leds[VERT_STRIP][LEDS_PER_STRIP - 1] = strips[1] >= threshold ? CHSV(current_hue % 255, DEFAULT_SATURATION, DEFAULT_INTENSITY) : CHSV(0, 0, 0);
}

void pulse_effect(bool is_inverse)
{
    int strips[4] = {0, 0, 0, 0};
    avg_freq_band_values(strips);

    // -------------------------------
    // Turn on LEDS
    // max and min decide the range that the mic's range, based on the input it makes a
    // percentage / max and then outputs a percentage / 30 which decides how many lights are on
    int boundaries[4][2] = {
        {get_level_boundary(LOW_MIN_LOWER, LOW_MAX_LOWER), get_level_boundary(LOW_MIN_UPPER, LOW_MAX_UPPER)},
        {get_level_boundary(LOWMID_MIN_LOWER, LOWMID_MAX_LOWER), get_level_boundary(LOWMID_MIN_UPPER, LOWMID_MAX_UPPER)},
        {get_level_boundary(HIGHMID_MIN_LOWER, HIGHMID_MAX_LOWER), get_level_boundary(HIGHMID_MIN_UPPER, HIGHMID_MAX_UPPER)},
        {get_level_boundary(HIGH_MIN_LOWER, HIGH_MAX_LOWER), get_level_boundary(HIGH_MIN_UPPER, HIGH_MAX_UPPER)},

    };

    for (int k = 0; k < NUM_STRIPS; k++)
    {
        if (strips[k] > boundaries[k][1])
            strips[k] = boundaries[k][1];

        if (strips[k] < boundaries[k][0])
            strips[k] = boundaries[k][0];

        byte val = map(strips[k], boundaries[k][0], boundaries[k][1], 0, LEDS_PER_STRIP);

        CHSV fills[2] = {
            CHSV(current_hue % 255, DEFAULT_SATURATION, DEFAULT_INTENSITY), // Solid color
            CHSV(0, 0, 0)                                                   // Black
        };

        for (int i = 0; i < val; i++)
        {
            leds[k][i] = fills[(int)is_inverse];
        }

        for (int i = val; i < LEDS_PER_STRIP; i++) // base state off
        {
            leds[k][i] = fills[(int)!is_inverse];
        }
    }
}

void color_effect()
{
    int strips[4] = {0, 0, 0, 0};
    avg_freq_band_values(strips);

    // -------------------------------
    // Turn on LEDS
    // max and min decide the range that the mic's range, based on the input it makes a
    // percentage / max and then outputs a percentage / 30 which decides how many lights are on
    int boundaries[4][2] = {
        {get_level_boundary(LOW_MIN_LOWER, LOW_MAX_LOWER), get_level_boundary(LOW_MIN_UPPER, LOW_MAX_UPPER)},
        {get_level_boundary(LOWMID_MIN_LOWER, LOWMID_MAX_LOWER), get_level_boundary(LOWMID_MIN_UPPER, LOWMID_MAX_UPPER)},
        {get_level_boundary(HIGHMID_MIN_LOWER, HIGHMID_MAX_LOWER), get_level_boundary(HIGHMID_MIN_UPPER, HIGHMID_MAX_UPPER)},
        {get_level_boundary(HIGH_MIN_LOWER, HIGH_MAX_LOWER), get_level_boundary(HIGH_MIN_UPPER, HIGH_MAX_UPPER)},

    };

    for (int k = 0; k < NUM_STRIPS; k++)
    {
        if (strips[k] > boundaries[k][1])
            strips[k] = boundaries[k][1];

        if (strips[k] < boundaries[k][0])
            strips[k] = boundaries[k][0];

        byte hue = map(strips[k], boundaries[k][0], boundaries[k][1], 0, 128);

        for (int i = 0; i < LEDS_PER_STRIP; i++)
        {
            leds[k][i] = CHSV((current_hue + hue) % 255, DEFAULT_SATURATION, DEFAULT_INTENSITY);
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