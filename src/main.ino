#include <FastLED.h>
#include "leds.h"
#include "utils.h"
#include "effects.h"

#define COLOR_ORDER RGB
#define CHIPSET WS2811
const char ENTROPY_PIN = A5;
const char LED_PIN = 7;
const char BRIGHTNESS = 255;

const char ARG_COUNT = 16;
const char hueTime = 30;
const unsigned long EFFECT_TIME = 60000;

const char refresh_period = 10;

unsigned long next_effect_at = 0;
unsigned char args[ARG_COUNT] = {0};
unsigned char rcvd_pos = 0;

// list of functions that will be displayed
const char EFFECT_COUNT = 4;
functionList effectList[] = {plasma, confetti, rider, slantBars};

void setup() {
  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS)
    .setCorrection(TypicalPixelString);
  FastLED.setBrightness(BRIGHTNESS);

  switch_to_effect(1);
  next_effect_at = millis() + EFFECT_TIME;
}

void loop() {
  if (millis() > next_effect_at) {
    switch_to_effect(currentEffect + 1);
    next_effect_at = millis() + EFFECT_TIME;
  }

  increment_hue();
  run_current_effect();
  FastLED.show();

  random16_add_entropy(analogRead(ENTROPY_PIN));
}

// switch to a new effect
void switch_to_effect(char effect_num) {
  currentEffect = effect_num;
  effectInit = false;
}

// increment the global hue value every hueTime milliseconds
void increment_hue() {
  if (millis() - hueMillis > hueTime) {
    hueMillis = millis();
    hueCycle(1);  // increment the global hue value
  }
}

// Run the currently selected effect every effectDelay milliseconds
void run_current_effect() {
  if (millis() - effectMillis > effectDelay) {
    effectMillis = millis();
    effectList[currentEffect]();  // run the selected effect function
  }
}
