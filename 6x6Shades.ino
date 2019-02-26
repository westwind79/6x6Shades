#define LED_PIN  6

#define COLOR_ORDER RGB
#define CHIPSET     WS2811

#define MAXBRIGHTNESS 150
#define STARTBRIGHTNESS 125

#define cycleTime 15000

#define hueTime 30
#define EEPROMDELAY 2000

#include <FastLED.h>
#include <EEPROM.h>
#include "palettes.h"
#include "messages.h"
#include "font.h"
#include "XYmap.h"
#include "utils.h"
#include "effects.h"
#include "buttons.h"


// list of functions that will be displayed
functionList effectList[] = {
  spirals,
  shadesOutline,
  scrollTextZero,
  rain,
  threeDee,
  colorFill,
  juggle,
  barfight,
  sideRain,
  BlacK_Blue_Magenta_WhiteNoise, 
  SunsetNoise, 
  es_vintage_57Noise, 
  CloudNoise, 
  ForestNoise, 
  amazingNoise,
  amazing,
  threeSine,  
  plasma,
  confetti,
  rider,
  scrollTextOne,
  glitter,
  slantBars,
  scrollTextTwo
};

const byte numEffects = (sizeof(effectList)/sizeof(effectList[0]));

// Runs one time at the start of the program (power up or reset)
void setup() {

  // check to see if EEPROM has been used yet
  // if so, load the stored settings
  byte eepromWasWritten = EEPROM.read(0);
  if (eepromWasWritten == 99) {
    currentEffect = EEPROM.read(1);
    autoCycle = EEPROM.read(2);
    currentBrightness = EEPROM.read(3);
  }

  if (currentEffect > (numEffects - 1)) currentEffect = 0;

  // write FastLED configuration data
  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, LAST_VISIBLE_LED + 1);

  // set global brightness value
  FastLED.setBrightness( scale8(currentBrightness, MAXBRIGHTNESS) );

  // configure input buttons
  pinMode(MODEBUTTON, INPUT_PULLUP);
  pinMode(BRIGHTNESSBUTTON, INPUT_PULLUP);

}


// Runs over and over until power off or reset
void loop()
{
  currentMillis = millis(); // save the current timer value
  updateButtons();          // read, debounce, and process the buttons
  doButtons();              // perform actions based on button state
  checkEEPROM();            // update the EEPROM if necessary

  // switch to a new effect every cycleTime milliseconds
  if (currentMillis - cycleMillis > cycleTime && autoCycle == true) {
    cycleMillis = currentMillis;
    if (++currentEffect >= numEffects) currentEffect = 0; // loop to start of effect list
    effectInit = false; // trigger effect initialization when new effect is selected
  }

  // increment the global hue value every hueTime milliseconds
  if (currentMillis - hueMillis > hueTime) {
    hueMillis = currentMillis;
    hueCycle(1); // increment the global hue value
  }

  // run the currently selected effect every effectDelay milliseconds
  if (currentMillis - effectMillis > effectDelay) {
    effectMillis = currentMillis;
    effectList[currentEffect](); // run the selected effect function
    random16_add_entropy(1); // make the random values a bit more random-ish
  }

  // run a fade effect too if the confetti effect is running
  if (effectList[currentEffect] == confetti) fadeAll(1);

  FastLED.show(); // send the contents of the led memory to the LEDs

}
