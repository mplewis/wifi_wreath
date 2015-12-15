#include <FastLED.h>
#include "leds.h"
#include "utils.h"
#include "effects.h"

#define COLOR_ORDER RGB
#define CHIPSET WS2811
const char ENTROPY_PIN = A5;
const char LED_PIN = 7;
const char BRIGHTNESS = 127;

const char BUF_SIZE = 64;
const char hueTime = 30; 

// fade-out rate. higher = longer, slower fades
const char fadeRate = 2;

char fadeCount = 0;
char rcvd[BUF_SIZE] = {0};
char rcvd_pos = 0;

// list of functions that will be displayed
#define NUM_EFFECTS 4
functionList effectList[] = {plasma, confetti, rider, slantBars};

void setup() {
  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS)
    .setCorrection(TypicalPixelString);
  FastLED.setBrightness(BRIGHTNESS);

  switch_to_effect(1);

  Serial.begin(115200);
}

void loop() {
  currentMillis = millis();

  increment_hue();
  increment_fade();
  run_current_effect();

  FastLED.show();

  random16_add_entropy(analogRead(ENTROPY_PIN));
}

// handle incoming serial data
void serialEvent() {
  while (Serial.available()) {
    char c = Serial.read();
    if (c == '\r'|| c == '\n') {
      process_cmd(rcvd_pos);
      rcvd_pos = 0;
    } else {
      rcvd[rcvd_pos] = c;
      rcvd_pos = (rcvd_pos + 1) % BUF_SIZE;
    }
  }
}

// process a finished serial command
void process_cmd(char length) {
  Serial.print("Received: ");
  for (char i = 0; i < length; i++) {
    Serial.print(rcvd[i]);
  }
  Serial.println();
}

// switch to a new effect every cycleTime milliseconds
void switch_to_effect(char effect_num) {
  currentEffect = effect_num;
  effectInit = false;
}

// increment the global hue value every hueTime milliseconds
void increment_hue() {
  if (currentMillis - hueMillis > hueTime) {
    hueMillis = currentMillis;
    hueCycle(1);  // increment the global hue value
  }
}

// Run the currently selected effect every effectDelay milliseconds
void run_current_effect() {
  if (currentMillis - effectMillis > effectDelay) {
    effectMillis = currentMillis;
    effectList[currentEffect]();  // run the selected effect function
  }
}

// run a fade effect too if the confetti effect is running
void increment_fade() {
  if (effectList[currentEffect] == confetti) {
    fadeCount++;
    if (fadeCount >= fadeRate) {
      fadeAll(1);
      fadeCount = 0;
    }
  }
}
