#include <FastLED.h>
#include "XYmap.h"
#include "utils.h"
#include "effects.h"
#include "buttons.h"

#define COLOR_ORDER RGB
#define CHIPSET WS2811
const int LED_PIN = 5;

const int MAXBRIGHTNESS = 72;
const int STARTBRIGHTNESS = 127;
// 0-255 will be scaled to 0-MAXBRIGHTNESS
byte currentBrightness = STARTBRIGHTNESS;
const int fadeRate = 5;

const int cycleTime = 15000;
const int hueTime = 30;

void setup() {
  // write FastLED configuration data
  // .setCorrection(TypicalSMD5050);
  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);

  // set global brightness value
  FastLED.setBrightness(scale8(STARTBRIGHTNESS, MAXBRIGHTNESS));

  // configure input buttons
  pinMode(MODEBUTTON, INPUT_PULLUP);
  pinMode(BRIGHTNESSBUTTON, INPUT_PULLUP);
  
  Serial.begin(115200);
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

  // Check the brightness adjust button
  switch (buttonStatus(1)) {
    case BTNRELEASED:           // button was pressed and released quickly
      currentBrightness += 51;  // increase the brightness (wraps to lowest)
      FastLED.setBrightness(scale8(currentBrightness, MAXBRIGHTNESS));
      break;

    case BTNLONGPRESS:                      // button was held down for a while
      currentBrightness = STARTBRIGHTNESS;  // reset brightness to startup value
      FastLED.setBrightness(scale8(currentBrightness, MAXBRIGHTNESS));
      break;
  }

  // switch to a new effect every cycleTime milliseconds
  if (currentMillis - cycleMillis > cycleTime && autoCycle == true) {
    cycleMillis = currentMillis;
    if (++currentEffect >= NUM_EFFECTS) currentEffect = 0;  // loop to start of effect list
    effectInit = false;  // trigger effect initialization when new effect is selected
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
  if (effectList[currentEffect] == confetti) fadeAll(1);

  FastLED.show();  // send the contents of the led memory to the LEDs

  if (currentMillis % 250 == 0) {
    Serial.print(currentMillis);
    Serial.print(", ");
    Serial.print(cycleMillis);
    Serial.print(", ");
    Serial.print(hueMillis);
    Serial.print(", ");
    Serial.print(effectMillis);
    Serial.print(", ");
    Serial.print(effectDelay);
    Serial.println();
  }
}
