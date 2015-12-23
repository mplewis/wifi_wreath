// indicates if a pattern has been recently switched
boolean effectInit = false;
// time between automatic effect changes
uint16_t effectDelay = 0;
// store the time of last effect function run
unsigned long effectMillis = 0;
// store the time of last effect change
unsigned long cycleMillis = 0;
// store current loop's millis value
unsigned long currentMillis;
// store time of last hue change
unsigned long hueMillis;

byte currentEffect = 0;    // index to the currently running effect
boolean autoCycle = true;  // flag for automatic effect changes

const char PALETTE_COUNT = 7;
CRGBPalette16 palettes[8] = {
  CloudColors_p, LavaColors_p, OceanColors_p, ForestColors_p, RainbowColors_p,
  PartyColors_p, HeatColors_p
};
CRGBPalette16 currentPalette = RainbowColors_p;  // global palette storage

// definition for list of effect function pointers
typedef void (*functionList)();
#define numEffects (sizeof(effectList) / sizeof(effectList[0]))

// Increment the global hue value for functions that use it
byte cycleHue = 0;
byte cycleHueCount = 0;
void hueCycle(byte incr) {
  cycleHueCount = 0;
  cycleHue += incr;
}

long paletteHue = 0;

// Set every LED in the array to a specified color
void fillAll(CRGB fillColor) {
  for (byte i = 0; i < NUM_LEDS; i++) {
    leds[i] = fillColor;
  }
}

// Fade every LED in the array by a specified amount
void fadeAll(byte fadeIncr) {
  for (byte i = 0; i < NUM_LEDS; i++) {
    leds[i] = leds[i].fadeToBlackBy(fadeIncr);
  }
}

// Interrupt normal operation to indicate that auto cycle mode has changed
void confirmBlink() {
  if (autoCycle) {  // one blue blink, auto mode active
    fillAll(CRGB::DarkBlue);
    FastLED.show();
    FastLED.delay(200);
    fillAll(CRGB::Black);
    FastLED.delay(200);
  } else {  // two red blinks, manual mode active
    fillAll(CRGB::DarkRed);
    FastLED.show();
    FastLED.delay(200);
    fillAll(CRGB::Black);
    FastLED.delay(200);
    fillAll(CRGB::DarkRed);
    FastLED.show();
    FastLED.delay(200);
    fillAll(CRGB::Black);
    FastLED.delay(200);
  }
}
