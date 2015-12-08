#define NUM_LEDS 100
CRGB leds[NUM_LEDS + 1];  // one dummy LED

// Randomized so that every LED is at least 7 apart from its neighbor
// Designed for use with a Christmas tree, so we assume the string doesn't
// wrap back around at the end
uint8_t randomized[] = {15, 90, 35, 52, 99, 21, 89, 78, 44, 19, 40, 77, 0, 22, 97, 50, 64, 74, 62, 24, 41, 68, 34, 98, 13, 94, 56, 38, 88, 61, 75, 7, 80, 37, 9, 69, 29, 51, 57, 23, 67, 42, 36, 6, 33, 45, 3, 60, 32, 70, 43, 73, 92, 30, 63, 48, 16, 86, 93, 85, 46, 72, 49, 95, 39, 17, 59, 12, 26, 83, 2, 27, 11, 1, 65, 20, 81, 31, 5, 71, 53, 79, 4, 10, 55, 14, 28, 47, 54, 84, 8, 58, 87, 76, 18, 96, 82, 25, 91, 66};

uint8_t led(uint8_t i) {
  if (i >= NUM_LEDS || i < 0) return NUM_LEDS;  // out of bounds -> dummy
  return randomized[i];
}
