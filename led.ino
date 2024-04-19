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

void rotate_led() // rotate function
{
    CRGB temp[LEDS_PER_STRIP];
    for (int strip = NUM_STRIPS - 1; strip > 0; --strip)
    {
        memcpy(temp, leds[strip], LEDS_PER_STRIP * sizeof(CRGB));
        memcpy(leds[strip], leds[strip - 1], LEDS_PER_STRIP * sizeof(CRGB));
        memcpy(leds[strip - 1], temp, LEDS_PER_STRIP * sizeof(CRGB));
    }
    // Increment rotate offset
    rotate_offset++;

    Serial.println("Rotate");
    led_off();
}

void pulse_effect() /// led update
{

    int strips[4] = {0, 0, 0, 0};

    // Split frequencies into sum per strip
    for (int i = 2; i < FHT_N / 2; i++)
    {
        if (i < FHT_N / 2 * 1 / 4)
            strips[0] += fht_log_out[i];
        else if (i < FHT_N / 2 * 2 / 4)
            strips[1] += fht_log_out[i];
        else if (i < FHT_N / 2 * 3 / 4)
            strips[2] += fht_log_out[i];
        else if (i < FHT_N / 2 * 4 / 4)
            strips[3] += fht_log_out[i];
    }

    // -------------------------------
    // Turn on LEDS
    // max and min decide the range that the mic's range, based on the input it makes a
    // percentage / max and then outputs a percentage / 30 which decides how many lights are on 
    int boundaries[4][2] = {
        {0, 500},
        {0, 400},
        {0, 400},
        {0, 400},
    };
    int strips_on = (rotate_task.isEnabled()) ? 1 : NUM_STRIPS; // Ternary if rotate on 1 if not 4 
    for (int j = 0; j < strips_on; j++) // change NUM_STRIPS to 1 for rotate
    {
        int k = (j + rotate_offset) % NUM_STRIPS;
        // int k = j;

        if (strips[k] > boundaries[j][1])
            strips[k] = boundaries[j][1];
        if (strips[k] < boundaries[j][0])
            strips[k] = boundaries[j][0];

        byte val = map(strips[k], boundaries[j][0], boundaries[j][1], 0, LEDS_PER_STRIP);
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