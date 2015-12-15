#define NUM_LEDS 50
CRGB leds[NUM_LEDS + 1];  // one dummy LED

// Randomized so that every LED is at least 7 apart from its neighbor
// Designed for use with a Christmas tree, so we assume the string doesn't
// wrap back around at the end
uint8_t randomized[] = {41, 31, 14, 26, 45, 6, 22, 3, 16, 44, 2, 48, 36, 5, 33, 1, 13, 34, 40, 20, 46, 15, 28, 47, 0, 35, 23, 42, 12, 29, 4, 39, 27, 21, 38, 49, 9, 24, 10, 30, 8, 18, 32, 43, 17, 25, 11, 19, 7, 37};

uint8_t led(uint8_t i) {
  if (i >= NUM_LEDS || i < 0) return NUM_LEDS;  // out of bounds -> dummy
  return randomized[i];
}
