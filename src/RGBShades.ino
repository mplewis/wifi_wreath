#include <FastLED.h>
#include "leds.h"
#include "utils.h"
#include "effects.h"

#define COLOR_ORDER RGB
#define CHIPSET WS2811
const byte ENTROPY_PIN = A5;
const byte LED_PIN = 7;
const byte BRIGHTNESS = 127;

const unsigned int cycleTime = 60000;
unsigned long midiTimeout = 30000;
const byte hueTime = 30; 

// fade-out rate. higher = longer, slower fades
const byte fadeRate = 2;

byte fadeCount = 0;

void setup() {
  // write FastLED configuration data
  // 
  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS)
    .setCorrection(TypicalPixelString);

  // set global brightness value
  FastLED.setBrightness(BRIGHTNESS);
}

// list of functions that will be displayed
#define NUM_EFFECTS 4
functionList effectList[] = {plasma, confetti, rider, slantBars};

// Runs over and over until power off or reset
void loop() {
  currentMillis = millis();  // save the current timer value

  // run a fade effect too if the confetti effect or MIDI is running
  if (effectList[currentEffect] == confetti) fadeCount++;
  if (fadeCount >= fadeRate) {
    fadeAll(1);
    fadeCount = 0;
  }

  FastLED.show();  // send the contents of the led memory to the LEDs

  // switch to a new effect every cycleTime milliseconds
  if (currentMillis - cycleMillis > cycleTime && autoCycle == true) {
    cycleMillis = currentMillis;
    // loop to start of effect list
    if (++currentEffect >= NUM_EFFECTS) currentEffect = 0;
    // trigger effect initialization when new effect is selected
    effectInit = false;
  }

  // increment the global hue value every hueTime milliseconds
  if (currentMillis - hueMillis > hueTime) {
    hueMillis = currentMillis;
    hueCycle(1);  // increment the global hue value
  }

  // Run the currently selected effect every effectDelay milliseconds
  if (currentMillis - effectMillis > effectDelay) {
    effectMillis = currentMillis;
    effectList[currentEffect]();  // run the selected effect function
  }

  random16_add_entropy(analogRead(ENTROPY_PIN));
}
