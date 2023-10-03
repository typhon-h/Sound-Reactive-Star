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

void pulse_effect() /// led update
{
    static int maxSensitivity = 255;
    static int minSensitivity = 0;

    int stripFrequencies[4] = {0, 0, 0, 0};

    for (int i = 2; i < FHT_N / 2; i++)
    {
        if (i < FHT_N / 2 * 1 / 4)
            stripFrequencies[0] += fht_log_out[i];
        else if (i < FHT_N / 2 * 2 / 4)
            stripFrequencies[1] += fht_log_out[i];
        else if (i < FHT_N / 2 * 3 / 4)
            stripFrequencies[2] += fht_log_out[i];
        else if (i < FHT_N / 2 * 4 / 4)
            stripFrequencies[3] += fht_log_out[i];
    }

    // Debug log for strip frequencies
    // Serial.print(strips[0]);
    // Serial.print(" ");
    // Serial.print(strips[1]);
    // Serial.print(" ");
    // Serial.print(strips[2]);
    // Serial.print(" ");
    // Serial.println(strips[3]);

    for (int k = 0; k < 4; k++)
    {
        if (stripFrequencies[k] > maxSensitivity)
            stripFrequencies[k] = maxSensitivity;
        if (stripFrequencies[k] < minSensitivity)
            stripFrequencies[k] = minSensitivity;

        byte val = map(stripFrequencies[k], minSensitivity, maxSensitivity, 0, LEDS_PER_STRIP);
        byte hue = 10;

        for (int i = 0; i < val; i++)
        {
            leds[k][i] = CHSV(hue += 10, DEFAULT_SATURATION, DEFAULT_INTENSITY);
        }

        for (int i = val - 1; i <= LEDS_PER_STRIP; i++)
        {
            leds[k][i] = CRGB::Black;
        }
    }

    FastLED.show(); // turn on and off for lights
}
