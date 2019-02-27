// Assorted useful functions and variables

// Global variables
boolean effectInit = false; // indicates if a pattern has been recently switched
uint16_t effectDelay = 0; // time between automatic effect changes
unsigned long effectMillis = 0; // store the time of last effect function run
unsigned long cycleMillis = 0; // store the time of last effect change
unsigned long currentMillis; // store current loop's millis value
unsigned long hueMillis; // store time of last hue change
unsigned long eepromMillis; // store time of last setting change
byte currentEffect = 0; // index to the currently running effect
boolean autoCycle = true; // flag for automatic effect changes
boolean eepromOutdated = false; // flag for when EEPROM may need to be updated
byte currentBrightness = STARTBRIGHTNESS; // 0-255 will be scaled to 0-MAXBRIGHTNESS
uint8_t fadeActive = 0;

CRGBPalette16 currentPalette(RainbowColors_p); // global palette storage

typedef void (*functionList)(); // definition for list of effect function pointers
extern const byte numEffects;

// Increment the global hue value for functions that use it
byte cycleHue = 0;
byte cycleHueCount = 0;
void hueCycle(byte incr) {
    cycleHueCount = 0;
    cycleHue+=incr;
}

// Set every LED in the array to a specified color
void fillAll(CRGB fillColor) {
  for (byte i = 0; i < NUM_LEDS; i++) {
    leds[i] = fillColor;
  }
}

// Fade every LED in the array by a specified amount
void fadeAll(byte fadeIncr) {
  for (byte i = 0; i < NUM_LEDS; i++) {
    leds[i] = leds[i].fadeToBlackBy(fadeIncr);
  }
}

// Shift all pixels by one, right or left (0 or 1)
void scrollArray(byte scrollDir) {
  
    byte scrollX = 0;
    for (byte x = 1; x < kMatrixWidth; x++) {
      if (scrollDir == 0) {
        scrollX = kMatrixWidth - x;
      } else if (scrollDir == 1) {
        scrollX = x - 1;
      }
      
      for (byte y = 0; y < kMatrixHeight; y++) {
        leds[XY(scrollX,y)] = leds[XY(scrollX + scrollDir*2 - 1,y)];
      }
    }
}

void scrollArray2(byte scrollDir) {
  
    byte scrollY = 0;
    for (byte y = 0; y < kMatrixHeight; y++) {
      if (scrollDir == 0) {
        scrollY = kMatrixHeight - y;
      } else if (scrollDir == 1) {
        scrollY = y;
      }
      
      for (byte x = 0; x < kMatrixWidth; x++) {
        leds[XY(x, scrollY)] = leds[XY(x, scrollY + scrollDir * 2 - 1)];
      }
    }
}

#define HOLD_PALETTES_X_TIMES_AS_LONG 1
static uint16_t x;
static uint16_t y;
static uint16_t z;

#define MAX_DIMENSION ((kMatrixWidth>kMatrixHeight) ? kMatrixWidth : kMatrixHeight)
uint8_t noise[MAX_DIMENSION][MAX_DIMENSION];
uint8_t colorLoop = 1;
uint16_t speed = 10; 
uint16_t scale = 40;

// Fill the x/y array of 8-bit noise values using the inoise8 function.
void fillnoise8() {
  // If we're runing at a low "speed", some 8-bit artifacts become visible
  // from frame-to-frame.  In order to reduce this, we can do some fast data-smoothing.
  // The amount of data smoothing we're doing depends on "speed".
  uint8_t dataSmoothing = 0;
  if( speed < 50) {
    dataSmoothing = 200 - (speed * 4);
  }
  
  for(int i = 0; i < MAX_DIMENSION; i++) {
    int ioffset = scale * i;
    for(int j = 0; j < MAX_DIMENSION; j++) {
      int joffset = scale * j;
      
      uint8_t data = inoise8(x + ioffset,y + joffset,z);

      // The range of the inoise8 function is roughly 16-238.
      // These two operations expand those values out to roughly 0..255
      // You can comment them out if you want the raw noise data.
      data = qsub8(data,16);
      data = qadd8(data,scale8(data,39));

      if( dataSmoothing ) {
        uint8_t olddata = noise[i][j];
        uint8_t newdata = scale8( olddata, dataSmoothing) + scale8( data, 256 - dataSmoothing);
        data = newdata;
      }
      
      noise[i][j] = data;
    }
  }
  
  z += speed;
  
  // apply slow drift to X and Y, just for visual variation.
  x += speed / 8;
  y -= speed / 16;
}

void mapNoiseToLEDsUsingPalette() {
  static uint8_t ihue=0;
  
  for(int i = 0; i < kMatrixWidth; i++) {
    for(int j = 0; j < kMatrixHeight; j++) {
      // We use the value at the (i,j) coordinate in the noise
      // array for our brightness, and the flipped value from (j,i)
      // for our pixel's index into the color palette.

      uint8_t index = noise[j][i];
      uint8_t bri =   noise[i][j];

      // if this palette is a 'loop', add a slowly-changing base value
      if( colorLoop) { 
        index += ihue;
      }

      // brighten up, as the color palette itself often contains the 
      // light/dark dynamic range desired
      if( bri > 127 ) {
        bri = 255;
      } else {
        bri = dim8_raw( bri * 2);
      }

      CRGB color = ColorFromPalette( currentPalette, index, bri);
      leds[XY(i,j)] = color;
    }
  }
  
  ihue+=1;
}

void SetupRandomPalette() {
  currentPalette = CRGBPalette16( 
                      CHSV( random8(), 255, 32), 
                      CHSV( random8(), 255, 255), 
                      CHSV( random8(), 128, 255), 
                      CHSV( random8(), 255, 255)); 
}

void SetupBlackAndWhiteStripedPalette() {
  // 'black out' all 16 palette entries...
  fill_solid( currentPalette, 16, CRGB::Black);
  // and set every fourth one to white.
  currentPalette[0] = CRGB::White;
  currentPalette[4] = CRGB::White;
  currentPalette[8] = CRGB::White;
  currentPalette[12] = CRGB::White;

}

// This function sets up a palette of purple and green stripes.
void SetupPurpleAndGreenPalette() {
  CRGB purple = CHSV( HUE_PURPLE, 255, 255);
  CRGB green  = CHSV( HUE_GREEN, 255, 255);
  CRGB black  = CRGB::Black;
  
  currentPalette = CRGBPalette16( 
    green,  green,  black,  black,
    purple, purple, black,  black,
    green,  green,  black,  black,
    purple, purple, black,  black );
}

void ChangePaletteAndSettingsPeriodically() {
  uint8_t secondHand = ((millis() / 1000) / HOLD_PALETTES_X_TIMES_AS_LONG) % 60;
  static uint8_t lastSecond = 99;
  
  if( lastSecond != secondHand) {
    lastSecond = secondHand;
    if( secondHand ==  0)  { currentPalette = RainbowColors_p;         speed = 20; scale = 30; colorLoop = 1; }
    if( secondHand ==  5)  { SetupPurpleAndGreenPalette();             speed = 10; scale = 50; colorLoop = 1; }
    if( secondHand == 10)  { SetupBlackAndWhiteStripedPalette();       speed = 20; scale = 30; colorLoop = 1; }
    if( secondHand == 15)  { currentPalette = ForestColors_p;          speed =  8; scale =120; colorLoop = 0; }
    if( secondHand == 20)  { currentPalette = CloudColors_p;           speed =  4; scale = 30; colorLoop = 0; }
    if( secondHand == 25)  { currentPalette = LavaColors_p;            speed =  8; scale = 50; colorLoop = 0; }
    if( secondHand == 30)  { currentPalette = OceanColors_p;           speed = 20; scale = 90; colorLoop = 0; }
    if( secondHand == 35)  { currentPalette = PartyColors_p;           speed = 20; scale = 30; colorLoop = 1; }
    if( secondHand == 40)  { SetupRandomPalette();                     speed = 20; scale = 20; colorLoop = 1; }
    if( secondHand == 45)  { SetupRandomPalette();                     speed = 50; scale = 50; colorLoop = 1; }
    if( secondHand == 50)  { SetupRandomPalette();                     speed = 90; scale = 90; colorLoop = 1; }
    if( secondHand == 55)  { currentPalette = RainbowStripeColors_p;   speed = 30; scale = 20; colorLoop = 1; }
  }
}

// Pick a random palette from a list
void selectRandomPalette() {

  switch(random8(19)) {
    case 0:
    currentPalette = CloudColors_p;
    break;
    
    case 1:
    currentPalette = LavaColors_p;
    break;
    
    case 2:
    currentPalette = OceanColors_p;
    break;
    
    case 4:
    currentPalette = ForestColors_p;
    break;
    
    case 5:
    currentPalette = RainbowColors_p;
    break;
    
    case 6:
    currentPalette = PartyColors_p;
    break;
    
    case 7:
    currentPalette = HeatColors_p;
    break;
    
    case 8:
    currentPalette = ib_jul01_gp;
    break;
    
    case 9:
    currentPalette = es_vintage_57_gp;
    break;
    
    case 10:
    currentPalette = es_vintage_01_gp;
    break;
    
    case 11:
    currentPalette = es_rivendell_15_gp;
    break;
    
    case 12:
    currentPalette = rgi_15_gp;
    break;
    
    case 13:
    currentPalette = es_emerald_dragon_08_gp;
    break;
    
    case 14:
    currentPalette = Magenta_Evening_gp;
    break;
    
    case 15:
    currentPalette = Sunset_Real_gp;
    break;
    
    case 16:
    currentPalette = BlacK_Blue_Magenta_White_gp;
    break;
    
    case 17:
    currentPalette = BlacK_Magenta_Red_gp;
    break;
    
    case 18:
    currentPalette = BlacK_Red_Magenta_Yellow_gp;
    break;
   
  }
}

void ChangePalette() {
  uint8_t secondHand = ((millis() / 1000) / HOLD_PALETTES_X_TIMES_AS_LONG) % 60;
  static uint8_t lastSecond = 99;
  
  if( lastSecond != secondHand) {
    lastSecond = secondHand;
    if( secondHand ==  0)  { selectRandomPalette();}
    if( secondHand ==  5)  { selectRandomPalette();}
    if( secondHand == 10)  { selectRandomPalette();}
    if( secondHand == 15)  { selectRandomPalette();}
    if( secondHand == 20)  { selectRandomPalette();}
    if( secondHand == 25)  { selectRandomPalette();}
    if( secondHand == 30)  { selectRandomPalette();}
    if( secondHand == 35)  { selectRandomPalette();}
    if( secondHand == 40)  { SetupRandomPalette(); }
    if( secondHand == 45)  { selectRandomPalette();}
    if( secondHand == 50)  { selectRandomPalette();}
    if( secondHand == 55)  { selectRandomPalette();}
  }
}

// Interrupt normal operation to indicate that auto cycle mode has changed
void confirmBlink() {

  if (autoCycle) { // one blue blink, auto mode active
    fillAll(CRGB::DarkBlue);
    FastLED.show();
    FastLED.delay(200);
    fillAll(CRGB::Black);
    FastLED.delay(200);
  } else { // two red blinks, manual mode active
    fillAll(CRGB::DarkRed);
    FastLED.show();
    FastLED.delay(200);
    fillAll(CRGB::Black);
    FastLED.delay(200);
    fillAll(CRGB::DarkRed);
    FastLED.show();
    FastLED.delay(200);
    fillAll(CRGB::Black);
    FastLED.delay(200);
  }

}

// Determine flash address of text string
unsigned int currentStringAddress = 0;
void selectFlashString(byte string) {
  currentStringAddress = pgm_read_word(&stringArray[string]);
}

// Fetch font character bitmap from flash
byte charBuffer[5] = {0};
void loadCharBuffer(byte character) {
  byte mappedCharacter = character;
  if (mappedCharacter >= 32 && mappedCharacter <= 95) {
    mappedCharacter -= 32; // subtract font array offset
  } else if (mappedCharacter >= 97 && mappedCharacter <= 122) {
    mappedCharacter -= 64; // subtract font array offset and convert lowercase to uppercase
  } else {
    mappedCharacter = 96; // unknown character block
  }
  
  for (byte i = 0; i < 5; i++) {
    charBuffer[i] = pgm_read_byte(Font[mappedCharacter]+i);
  }
  
}

// Fetch a character value from a text string in flash
char loadStringChar(byte string, byte character) {
  return (char) pgm_read_byte(currentStringAddress + character);
}

// write EEPROM value if it's different from stored value
void updateEEPROM(byte location, byte value) {
  if (EEPROM.read(location) != value) EEPROM.write(location, value);
}

// Write settings to EEPROM if necessary
void checkEEPROM() {
  if (eepromOutdated) {
    if (currentMillis - eepromMillis > EEPROMDELAY) {
      updateEEPROM(0, 99);
      updateEEPROM(1, currentEffect);
      updateEEPROM(2, autoCycle);
      updateEEPROM(3, currentBrightness);
      eepromOutdated = false;
    }
  }
}
