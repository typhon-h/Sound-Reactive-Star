#include <FHT.h> // FFT-like library
#include "circularBuffer.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>

static circBuf_t g_micSampleBuffer;

void microphone_setup(void)
{
    ADMUX = (1 << REFS0); // Set reference voltage (AVCC with external capacitor at AREF pin)

    // Set ADC prescaler to achieve desired clock rate
    ADCSRA = (1 << ADPS1) | (1 << ADPS0); // Prescaler of 4 for 16 MHz clock (approximately 62.5 kHz ADC clock)

    ADCSRA |= (1 << ADEN) | (1 << ADIE); // Enable ADC and ADC interrupt

    PCICR |= (1 << PCIE1);   // Enable pin change interrupt for PCINT8 (PC0)
    PCMSK1 |= (1 << PCINT8); // Enable interrupt on PC0

    initCircBuf(&g_micSampleBuffer, FHT_N);
}

ISR(PCINT1_vect)
{
    // Start ADC conversion
    ADCSRA |= (1 << ADSC);
}

ISR(ADC_vect)
{
    byte m = ADCL; // fetch adc data
    byte j = ADCH;
    int16_t k = (j << 8) | m; // form into an int

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