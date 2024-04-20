/** @file microphone.h
 * @author Leonardo Bolstad
 * @date 04 Oct 2023
 * @brief variables regarding microphone
 */
#ifndef MIC_H
#define MIC_H

#define LOG_OUT 1 // LOG output
#define FHT_N 64  // 128 samples
#include <FHT.h>  // FFT-like library

#define MICROPHONE_PIN A0

// Breakdown of frequency bands in fht_mag_output
#define NOISE_INDEX 1     // 0-1 unused
#define LOW_INDEX 5       // 2-5 lows 0-500hz
#define LOW_MID_INDEX 9   // 6-9 low mids 500-1000hz
#define HIGH_MID_INDEX 18 // 10-18 high mids 1000-2000hz
#define HIGH_INDEX 31     // 19-31 high 2000-4000hz

#define SAMPLE_RATE 9600   // Max ADC sample rate (Hz)
#define LOW_PASS_FREQ 4000 // Cutoff frequency of the low-pass filter (Hz)

// Define ranges for band averages at min/ max sensitivity
#define LOW_MIN_LOWER 10
#define LOW_MIN_UPPER 25
#define LOW_MAX_LOWER 30
#define LOW_MAX_UPPER 90

#define LOWMID_MIN_LOWER 2
#define LOWMID_MIN_UPPER 22
#define LOWMID_MAX_LOWER 30
#define LOWMID_MAX_UPPER 90

#define HIGHMID_MIN_LOWER 6
#define HIGHMID_MIN_UPPER 12
#define HIGHMID_MAX_LOWER 25
#define HIGHMID_MAX_UPPER 90

#define HIGH_MIN_LOWER 0
#define HIGH_MIN_UPPER 7
#define HIGH_MAX_LOWER 20
#define HIGH_MAX_UPPER 70

#define DELTA_SENSITIVITY 0.1f // TODO: Maybe make this 0.05 for precision
float mic_sensitivity = 0.8f;  // Band sensitivity multiplier between 0-1

int get_level_boundary(int lower, int upper);

void microphone_setup(void);
void microphone_sample(void);
void avg_freq_band_values(int *bands);

#endif // MIC_H