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

int readAudioWithFilter()
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