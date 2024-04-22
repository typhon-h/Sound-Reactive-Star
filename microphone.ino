/** @file microphone.ino
 * @author Leonardo Bolstad
 * @date 04 Oct 2023
 * @brief code regarding microphone
 */
#include "microphone.h"
#include <FHT.h> // FFT-like library
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>

void microphone_setup(void)
{
}

int readAudioWithFilter() // Apply a low pass filter to reduce aliasing
{
    static float lastValue = 0;                                           // Previous filtered value
    float alpha = LOW_PASS_FREQ / (LOW_PASS_FREQ + SAMPLE_RATE * 2 * PI); // Filter coefficient

    // Read audio sample from microphone
    int rawValue = analogRead(MICROPHONE_PIN);
    float newValue = rawValue * (1 - alpha) + lastValue * alpha; // Low-pass filtering

    lastValue = newValue; // Update last filtered value

    return newValue; // Return filtered audio sample
}

void microphone_sample()
{
    cli();
    for (int i = 0; i < FHT_N; i++)
    {
        int k = readAudioWithFilter();
        fht_input[i] = k; // fill out array
    }
    fht_window();  // window the data for better frequency response
    fht_reorder(); // reorder the data before doing the fht
    fht_run();     // process the data in the fht
    fht_mag_log(); // take the output of the fht
    sei();

    // For Microphone level visualizer
    // for (int i = 0; i < (FHT_N / 2) - 1; i++)
    // {
    //     Serial.print(fht_log_out[i]);
    //     Serial.print(",");
    // }
    // Serial.println(fht_log_out[(FHT_N / 2) - 1]);
}

void avg_freq_band_values(int *bands)
{
    for (int i = NOISE_INDEX + 1; i <= LOW_INDEX; i++)
    {
        bands[0] += fht_log_out[i] / (LOW_INDEX - NOISE_INDEX);
    }

    for (int i = LOW_INDEX + 1; i <= LOW_MID_INDEX; i++)
    {
        bands[1] += fht_log_out[i] / (LOW_MID_INDEX - LOW_INDEX);
    }

    for (int i = LOW_MID_INDEX + 1; i <= HIGH_MID_INDEX; i++)
    {
        bands[2] += fht_log_out[i] / (HIGH_MID_INDEX - LOW_MID_INDEX);
    }

    for (int i = HIGH_MID_INDEX + 1; i <= HIGH_INDEX; i++)
    {
        bands[3] += fht_log_out[i] / (HIGH_INDEX - HIGH_MID_INDEX);
    }

    return bands;
}

int get_level_boundary(int lower, int upper)
{
    return upper + (lower - upper) * mic_sensitivity;
}