/** @file microphone.h
 * @author Leonardo Bolstad
 * @date 04 Oct 2023
 * @brief variables regarding microphone
 */
#ifndef MIC_H
#define MIC_H

#define LOG_OUT 1 // LOG output
#define FHT_N 64  // 128 samples

// Breakdown of frequency bands in fht_mag_output
#define NOISE_INDEX 1     // 0-1 unused
#define LOW_INDEX 5       // 2-5 lows 0-500hz
#define LOW_MID_INDEX 9   // 6-9 low mids 500-1000hz
#define HIGH_MID_INDEX 18 // 10-18 high mids 1000-2000hz
#define HIGH_INDEX 31     // 19-31 high 2000-4000hz

#define SAMPLE_RATE 9600   // Max ADC sample rate (Hz)
#define LOW_PASS_FREQ 4000 // Cutoff frequency of the low-pass filter (Hz)

#include <FHT.h> // FFT-like library

void microphone_setup(void);
void microphone_sample(void);
void avg_freq_band_values(int *bands);

#define MICROPHONE_PIN A0

#endif // MIC_H