/**
 * @brief Initializes the strips within FastLED with default settings.
 *
 */
void led_setup()
{
    FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, LED_COUNT);
    FastLED.setBrightness(DEFAULT_INTENSITY);
    FastLED.setCorrection(TypicalLEDStrip);
    FastLED.clear();
    FastLED.show();
}