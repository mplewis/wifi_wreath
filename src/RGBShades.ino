#include <FastLED.h>
#include "XYmap.h"
#include "utils.h"
#include "effects.h"
#include "buttons.h"

#define COLOR_ORDER RGB
#define CHIPSET WS2811
#define BRIGHTNESS_DIAL A0
const byte LED_PIN = 5;

const int cycleTime = 60000;
const byte hueTime = 30;
const byte fadeRate = 2;

byte fadeCount = 0;

void setup() {
  // write FastLED configuration data
  // .setCorrection(TypicalSMD5050);
  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);

  // set global brightness value
  FastLED.setBrightness(255);

  // configure input buttons
  pinMode(MODEBUTTON, INPUT_PULLUP);
  pinMode(BRIGHTNESSBUTTON, INPUT_PULLUP);
  pinMode(PALETTEBUTTON, INPUT_PULLUP);
  pinMode(BRIGHTNESS_DIAL, INPUT);
  pinMode(ENTROPY_PIN, INPUT);
}

// list of functions that will be displayed
#define NUM_EFFECTS 4
functionList effectList[] = {plasma, confetti, rider, slantBars};

// Runs over and over until power off or reset
void loop() {
  currentMillis = millis();  // save the current timer value
  updateButtons();           // read, debounce, and process the buttons

  // Check the mode button (for switching between effects)
  switch (buttonStatus(0)) {
    case BTNRELEASED:  // button was pressed and released quickly
      cycleMillis = currentMillis;
      if (++currentEffect >= NUM_EFFECTS)
        currentEffect = 0;  // loop to start of effect list
      effectInit =
          false;  // trigger effect initialization when new effect is selected
      break;

    case BTNLONGPRESS:         // button was held down for a while
      autoCycle = !autoCycle;  // toggle auto cycle mode
      confirmBlink();  // one blue blink: auto mode. two red blinks: manual
                       // mode.
      break;
  }

  // Check the palette switch button
  switch (buttonStatus(2)) {
    case BTNRELEASED:           // button was pressed and released quickly
      selectRandomPalette();
      fill_solid(leds, NUM_LEDS, CRGB::Black);
      break;
  }

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

  // run the currently selected effect every effectDelay milliseconds
  if (currentMillis - effectMillis > effectDelay) {
    effectMillis = currentMillis;
    effectList[currentEffect]();  // run the selected effect function
    random16_add_entropy(1);  // make the random values a bit more random-ish
  }

  // run a fade effect too if the confetti effect is running
  if (effectList[currentEffect] == confetti) fadeCount++;
  if (fadeCount >= fadeRate) {
    fadeAll(1);
    fadeCount = 0;
  }

  // check the brightness dial
  if (currentMillis % 10 == 0) {
    int dial_raw = analogRead(BRIGHTNESS_DIAL);
    byte dial = map(dial_raw, 0, 1023, 0, 255);
    byte dimmed = dim8_raw(dial);
    FastLED.setBrightness(dimmed);
  }

  FastLED.show();  // send the contents of the led memory to the LEDs
}
