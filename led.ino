/**
 * @brief Initializes the strips within FastLED with default settings.
 *
 */
void led_setup()
{
    FastLED.addLeds<NEOPIXEL, VERT_STRIP_PIN>(leds[VERT_STRIP], LEDS_PER_STRIP);
    FastLED.addLeds<NEOPIXEL, RSLANT_STRIP_PIN>(leds[RSLANT_STRIP], LEDS_PER_STRIP);
    FastLED.addLeds<NEOPIXEL, HORIZ_STRIP_PIN>(leds[HORIZ_STRIP], LEDS_PER_STRIP);
    FastLED.addLeds<NEOPIXEL, LSLANT_STRIP_PIN>(leds[LSLANT_STRIP], LEDS_PER_STRIP);

    FastLED.setBrightness(DEFAULT_INTENSITY);
    FastLED.setCorrection(TypicalLEDStrip);
    FastLED.clear();
    FastLED.show();
}


void color_cycle()
{
  active_hue++;
  for(int i = 0; i < NUM_STRIPS; i++) {
    fill_solid(leds[i], LEDS_PER_STRIP, CHSV(active_hue++, DEFAULT_SATURATION, DEFAULT_INTENSITY));
  }
  
  FastLED.show();
}