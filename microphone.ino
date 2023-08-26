#include "circularBuffer.h"
#include "microphone.h"
#include <FHT.h> // FFT-like library
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>

static CircBuf_t g_micSampleBuffer;

void microphone_setup(void)
{
    // Set A0 pin as input
    DDRF &= ~(1 << DDF7); // Pin A0 corresponds to port F and pin 7

    ADMUX = (1 << REFS0); // Set reference voltage (AVCC with external capacitor at AREF pin)

    // Set ADC prescaler to achieve desired clock rate
    ADCSRA = (1 << ADPS1) | (1 << ADPS0); // Prescaler of 4 for 16 MHz clock (approximately 62.5 kHz ADC clock)

    ADCSRA |= (1 << ADEN) | (1 << ADIE); // Enable ADC and ADC interrupt

    // Start ADC conversion
    ADCSRA |= (1 << ADSC);

    initCircBuf(&g_micSampleBuffer, FHT_N);
}

ISR(ADC_vect)
{
    int k = (ADCH << 8) | ADCL; // form into an int

    k -= 0x0200; // form into a signed int
    k <<= 6;     // form into a 16b signed int

    writeCircBuf(&g_micSampleBuffer, k);

    ADCSRA = 0xf5; // restart adc
}

void microphone_sample()
{
    cli();

    for (int i = 0; i < FHT_N; i++)
    {
        fht_input[i] = readCircBuf(&g_micSampleBuffer);
    }

    fht_window();
    fht_reorder();
    fht_run();
    fht_mag_log();

    sei();
}
