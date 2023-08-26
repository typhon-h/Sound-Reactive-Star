#include "microphone.h"
#include <FHT.h> // FFT-like library
#include "led.h"

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
    FastLED.clear();
    FastLED.show();
}

void color_cycle()
{

    int strip_height[4] = {0, 0, 0, 0};

    for (int i = 2; i < FHT_N / 2; i++)
    {
        // clear out anything that isn't better than 32
        if (fht_log_out[i] < 32)
        {
            fht_log_out[i] = 0;
        }
        else
        {
            fht_log_out[i] -= 32; // reduce by 32 just to get a clearer image
        }

        fht_log_out[i] *= 2; // double the strength of it (*2)

        Serial.print(fht_log_out[i]);
        Serial.print(',');

        if (i < FHT_N / 2 * 1 / 4)
            strip_height[0] += fht_log_out[i];
        if (i < FHT_N / 2 * 2 / 4)
            strip_height[1] += fht_log_out[i];
        if (i < FHT_N / 2 * 3 / 4)
            strip_height[2] += fht_log_out[i];
        if (i < FHT_N / 2 * 4 / 4)
            strip_height[3] += fht_log_out[i];
    }
    Serial.print("\n");

    // for the below specific frequency bins, we'll put a value in one of the
    // row displays. this is to represent the value at that particular
    // height.

    // for each pixel, figure out row, and if it should be coloured or not.

    for (int k = 0; k < 4; k++)
    {
        byte val = map(strip_height[k], 0, 255, 0, LEDS_PER_STRIP);
        byte hue = 10;

        for (int i = 0; i <= val; i++)
        {
            leds[k][i] = CHSV(hue += 10, DEFAULT_SATURATION, DEFAULT_INTENSITY);
        }

        for (int i = val + 1; i <= LEDS_PER_STRIP; i++)
        {
            leds[k][i].nscale8(10);
        }
    }

    FastLED.show();
}
