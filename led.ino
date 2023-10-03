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

    int strips[4] = {0, 0, 0, 0};

    for (int i = 0; i < FHT_N; i++)
    {
        int k = analogRead(A0);

        fht_input[i] = k; // fill out array
    }

    fht_window();  // window the data for better frequency response
    fht_reorder(); // reorder the data before doing the fht
    fht_run();     // process the data in the fht
    fht_mag_log(); // take the output of the fht

    // 8 bins to break up the frequencies - map them to 4 strips
    for (int i = 2; i < FHT_N / 2; i++)
    {
        if (i < FHT_N / 2 * 1 / 8)
            strips[0] += fht_log_out[i];
        if (i < FHT_N / 2 * 3 / 8)
            strips[1] += fht_log_out[i];
        if (i < FHT_N / 2 * 5 / 8)
            strips[2] += fht_log_out[i];
        if (i < FHT_N / 2 * 7 / 8)
            strips[3] += fht_log_out[i];
    }

    Serial.print(strips[0]);
    Serial.print(" ");
    Serial.print(strips[1]);
    Serial.print(" ");
    Serial.print(strips[2]);
    Serial.print(" ");
    Serial.println(strips[3]);

    // -------------------------------
    // Turn on LEDS

    // for the below specific frequency bins, we'll put a value in one of the
    // row displays. this is to represent the value at that particular
    // height.

    // for each pixel, figure out row, and if it should be coloured or not.

    // for (int k = 0; k < 4; k++)
    // {
    //     byte val = map(strip_height[k], 0, 255, 0, LEDS_PER_STRIP);
    //     byte hue = 10;

    //     for (int i = 0; i <= val; i++)
    //     {
    //         leds[k][i] = CHSV(hue += 10, DEFAULT_SATURATION, DEFAULT_INTENSITY);
    //     }

    //     for (int i = val + 1; i <= LEDS_PER_STRIP; i++)
    //     {
    //         leds[k][i].nscale8(10);
    //     }
    // }

    FastLED.show();
}
