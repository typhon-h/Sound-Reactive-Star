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
    // Do something
}

void microphone_sample()
{
    for (int i = 0; i < FHT_N; i++)
    {
        int k = analogRead(MICROPHONE_PIN);
        // Serial.println(k);
        fht_input[i] = k; // fill out array
    }

    fht_window();  // window the data for better frequency response
    fht_reorder(); // reorder the data before doing the fht
    fht_run();     // process the data in the fht
    fht_mag_log(); // take the output of the fht

    // 8 bins to break up the frequencies - map them to 4 strips
    for (int i = 2; i < FHT_N / 2; i++)
    {
        if (fht_log_out[i] < 32)
        {
            fht_log_out[i] = 0;
        }
        else
        {
            fht_log_out[i] -= 32;
        }
    }
}