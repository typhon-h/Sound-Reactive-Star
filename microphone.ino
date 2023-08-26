#include "circularBuffer.h"
#include "microphone.h"
#include <FHT.h> // FFT-like library
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>

CircBuf_t g_micSampleBuffer;

void microphone_setup(void)
{
    initCircBuf(&g_micSampleBuffer, FHT_N);
}

void microphone_sample()
{
    writeCircBuf(&g_micSampleBuffer, analogRead(A0));
}
