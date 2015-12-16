#include <FastLED.h>
#include "mqtt.h"
#include "leds.h"
#include "utils.h"
#include "effects.h"

#define COLOR_ORDER RGB
#define CHIPSET WS2811
const char ENTROPY_PIN = A5;
const char LED_PIN = 7;
const char BRIGHTNESS = 127;

const char BUF_SIZE = 64;
const char ARG_COUNT = 16;
const char hueTime = 30; 

// fade-out rate. higher = longer, slower fades
const char fadeRate = 2;

char fadeCount = 0;
char rcvd[BUF_SIZE] = {0};
unsigned char args[ARG_COUNT] = {0};
unsigned char rcvd_pos = 0;

// list of functions that will be displayed
functionList effectList[] = {static_color, plasma, confetti, rider, slantBars};

void setup() {
  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS)
    .setCorrection(TypicalPixelString);
  FastLED.setBrightness(BRIGHTNESS);

  switch_to_effect(0);

  mqtt_setup();
}

void loop() {
  currentMillis = millis();

  increment_hue();
  increment_fade();
  run_current_effect();

  FastLED.show();

  random16_add_entropy(analogRead(ENTROPY_PIN));
  mqtt_process();
}

// process a finished serial command
void process_cmd(char length) {
  char *as_chars = strtok(rcvd, ",");
  int arg_num = 0;
  while (as_chars != 0) {
    args[arg_num++] = atoi(as_chars);
    as_chars = strtok(0, ",");
  }

  if (args[0] == 1) {
    // switch effect
    switch_to_effect(args[1]);
    Serial.print("Set effect: ");
    Serial.println(args[1], DEC);

  } else if (args[0] == 2) {
    // switch palette
    currentPalette = palettes[(unsigned char)args[1]];
    Serial.print("Set palette: ");
    Serial.println(args[1], DEC);

  } else if (args[0] == 3) {
    // set brightness
    FastLED.setBrightness(args[1]);
    Serial.print("Set brightness: ");
    Serial.println(args[1], DEC);

  } else if (args[0] == 4) {
    // set static color
    switch_to_effect(0);
    curr_static_color.r = args[1];
    curr_static_color.g = args[2];
    curr_static_color.b = args[3];
    Serial.print("Set static color: ");
    Serial.print(curr_static_color.r, DEC);
    Serial.print(", ");
    Serial.print(curr_static_color.g, DEC);
    Serial.print(", ");
    Serial.println(curr_static_color.b, DEC);

  } else {
    Serial.print("Unrecognized action: ");
    Serial.println(args[0], DEC);
  }
}

// switch to a new effect
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
