#define NUM_LEDS 50
CRGB leds[NUM_LEDS + 1];  // one dummy LED

// Randomized so that every LED is at least 7 apart from its neighbor
// Designed for use with a Christmas tree, so we assume the string doesn't
// wrap back around at the end
uint8_t randomized[] = {42, 29, 37, 16, 24, 35, 20, 9,  2,  21, 33, 25, 5,
                        49, 15, 38, 22, 43, 36, 8,  47, 12, 45, 1,  10, 19,
                        41, 6,  13, 40, 0,  44, 32, 48, 39, 30, 23, 46, 31,
                        3,  27, 7,  18, 34, 14, 28, 11, 26, 4,  17};

uint8_t led(uint8_t i) {
  if (i >= NUM_LEDS || i < 0) return NUM_LEDS;  // out of bounds -> dummy
  return randomized[i];
}
