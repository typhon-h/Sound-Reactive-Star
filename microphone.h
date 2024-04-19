/** @file microphone.h
 * @author Leonardo Bolstad
 * @date 04 Oct 2023
 * @brief variables regarding microphone
 */
#ifndef MIC_H
#define MIC_H

#define LOG_OUT 1 // LOG output
#define FHT_N 64  // 128 samples

#define SAMPLE_RATE 9600   // Max ADC sample rate (Hz)
#define LOW_PASS_FREQ 4000 // Cutoff frequency of the low-pass filter (Hz)

#include <FHT.h> // FFT-like library

void microphone_setup(void);
void microphone_sample(void);

#define MICROPHONE_PIN A0

#endif // MIC_H