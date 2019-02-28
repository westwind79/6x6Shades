CRGB leds2[NUM_LEDS];
CRGB leds3[NUM_LEDS];

void animationA() {
  // running red stripes 
  for (uint16_t i = 0; i < NUM_LEDS; i++) {
    uint8_t red = (millis() / 3) + (i * 5);
    if (red > 128) red = 0;
    leds2[i] = CRGB(red, 0, 0);
  }
}

void animationB() {
  // the moving rainbow
  for (uint16_t i = 0; i < NUM_LEDS; i++) {
    leds3[i] = CHSV((millis() / 4) - (i * 3), 255, 255);
  }
}
void twoAnimations() {
  
 if (effectInit == false) {
		effectInit = true;
		effectDelay = 45;
		fadeActive = 0;
 }
  // render the first animation into leds2 
  animationA();
  
  // render the second animation into leds3
  animationB();

  // set the blend ratio for the video cross fade
  // (set ratio to 127 for a constant 50% / 50% blend)
  uint8_t ratio = beatsin8(5);

  // mix the 2 arrays together
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = blend( leds2[i], leds3[i], ratio );
  }

}

void spinPlasma() {
 
 static byte offset  = 0;
 static int plasVector = 10;

 if (effectInit == false) {
		effectInit = true;
		effectDelay = 45;
		fadeActive = 0;
 }
	ChangePalette();
 // Calculate current center of plasma pattern (can be offscreen)
 int xOffset = cos8(plasVector / 256);
 int yOffset = sin8(plasVector / 256);

 //int xOffset = 0;
 //int yOffset = 0;
 
 // Draw one frame of the animation into the LED array
 for (int x = 0; x < kMatrixWidth; x++) {
	 for (int y = 0; y < kMatrixHeight; y++) {
		 byte color = sin8(sqrt(sq(((float)x - 8) * 12 + xOffset) + sq(((float)y - 16) * 12 + yOffset)) + offset);
		 leds[XY(x, y)] = ColorFromPalette(currentPalette, color, 255);
	 }
 }
 offset++;
 plasVector += 1;
 Serial.print("spinPlasma");
}

#define rainDir3 1
void rainUp() { 

	if (effectInit == false) {
		effectInit = true;
		effectDelay = 45;
		// currentPalette = CloudColors_p;
		fadeActive = 1;
	}
	
	scrollArray2(rainDir3);
	byte randPixel = random8(kMatrixWidth);
	for (byte x = 0; x < kMatrixWidth; x++) {
		leds[XY(x, (kMatrixHeight - 1) * rainDir3)] = CRGB::Black;
	}
	leds[XY(randPixel, (kMatrixHeight - 1)*rainDir3)] = CHSV(cycleHue, 255, 255);
	// leds[XY(randPixel, (kMatrixHeight - 1)*rainDir3)] = ColorFromPalette(currentPalette, 240, 255);
}

void twoAnimations2() {
  
 if (effectInit == false) {
		effectInit = true;
		effectDelay = 45;
		fadeActive = 0;
 }
  // render the first animation into leds2 
  spinPlasma();
  
  // render the second animation into leds3
  rainUp();

  // set the blend ratio for the video cross fade
  // (set ratio to 127 for a constant 50% / 50% blend)
  uint8_t ratio = beatsin8(5);

  // mix the 2 arrays together
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = blend( leds2[i], leds3[i], ratio );
  }

}
#define rainDir4 0
void rainDown() {

	// startup tasks
	if (effectInit == false) {
		effectInit = true;
		effectDelay = 90;
		currentPalette = CloudColors_p;
		fadeActive = 1;
	}

	scrollArray2(rainDir4);
	byte randPixel = random8(kMatrixWidth);
	for (byte x = 0; x < kMatrixWidth; x++) {
		leds[XY(x, (kMatrixHeight - 1) * rainDir4)] = CRGB::Black;
	}
//  leds[XY(randPixel, (kMatrixHeight - 1)*rainDir4)] = CHSV(255, 255, 255);
		leds[XY(randPixel, (kMatrixHeight - 1)*rainDir4)] = ColorFromPalette(currentPalette, 240, 255);
}

#define rainDir4 0
void rainDown2() {

	// startup tasks
	if (effectInit == false) {
		effectInit = true;
		effectDelay = 60;
		fadeActive = 1;
	}
 
	CRGBPalette16 palette = CloudColors_p;

	scrollArray2(rainDir4);
	byte randPixel = random8(kMatrixWidth);
	for (byte x = 0; x < kMatrixWidth; x++) {
		leds[XY(x, (kMatrixHeight - 1) * rainDir4)] = CRGB::Black;
	}
	leds[XY(randPixel, (kMatrixHeight - 1)*rainDir4)] = ColorFromPalette(palette, 240, 240);
}

void snow() {

	static unsigned int snowCols[kMatrixWidth] = {0};

	// startup tasks
	if (effectInit == false) {
		effectInit = true;
		effectDelay = 20;
		fadeActive = 1;
	}

	CRGB snowColor = CRGB::White;

	FastLED.clear();

	for (byte i = 0; i < kMatrixWidth; i++) {
		if (snowCols[i] > 0) {
			snowCols[i] += random(4,16);
		} else {
			if (random8(0,100) == 0) snowCols[i] = 1;
		}
		byte tempY = snowCols[i] >> 8;
		byte tempRem = snowCols[i] & 0xFF;
		if (tempY <= kMatrixHeight) leds[XY(i,tempY-1)] = snowColor % dim8_raw(255-tempRem);
		if (tempY < kMatrixHeight) leds[XY(i,tempY)] = snowColor % dim8_raw(tempRem);
		if (tempY > kMatrixHeight) snowCols[i] = 0;
	}
}


int wave1=0;
int wave2=0;
int wave3=0;
uint8_t mul1 = 5;
uint8_t mul2 = 6;
uint8_t mul3 = 3;

void crazyRainbowV(){ 
 if (effectInit == false) {
	 effectInit = true;
	 effectDelay = 90;
	// selectRandomPalette();
	 // CRGBPalette16 targetPalette(PartyColors_p);
	 fadeActive = 0;
 }
	ChangePalette();
 wave1 += beatsin8(1,-2,1);
 wave2 += beatsin8(3,-2,3);
 wave3 += beatsin8(2,-1, 2);
 for (byte x = 0; x<kMatrixWidth; x++) {
	 for (int j=0; j<kMatrixHeight; j++) {
			uint8_t tmp = sin8(mul1*j + wave1) + sin8(mul1*j + wave2) + sin8(mul1*j + wave3);
			leds[XY(x,j)] = ColorFromPalette(currentPalette, tmp, 255);
		}
	}
}

void crazyRainbowH(){ 
 if (effectInit == false) {
	 effectInit = true;
	 effectDelay = 60;
	 selectRandomPalette();
	 // CRGBPalette16 targetPalette(PartyColors_p);
	 fadeActive = 0;
 }
 wave1 += beatsin8(2,-4,2);
 wave2 += beatsin8(1,-2,1);
 wave3 += beatsin8(2,-3, 3);
 
 for (byte x = 0; x<kMatrixHeight; x++) {
	 for (int j=0; j<kMatrixWidth; j++) {
		 uint8_t tmp = sin8(mul1*j + wave1) + sin8(mul1*j + wave2) + sin8(mul1*j + wave3);
		 leds[XY(j,x)] = ColorFromPalette(currentPalette, tmp, 255);
	 }
 }
}

void threeSineVert() {

	static byte sineOffset = 0; // counter for current position of sine waves

	// startup tasks
	if (effectInit == false) {
		effectInit = true;
		effectDelay = 60;
		fadeActive = 0;
	}

	// Draw one frame of the animation into the LED array
	for (byte x = 0; x < kMatrixWidth; x++) {
		for (int y = 0; y < kMatrixHeight; y++) {

			// Calculate "sine" waves with varying periods
			// sin8 is used for speed; cos8, quadwave8, or triwave8 would also work here
			byte sinDistanceR = qmul8(abs(x * (255 / kMatrixWidth) - cos8(sineOffset * 9 + y * 4)), 2);
			byte sinDistanceG = qmul8(abs(x * (255 / kMatrixWidth) - cos8(sineOffset * 10 + y * 4)), 2);
			byte sinDistanceB = qmul8(abs(x * (255 / kMatrixWidth) - cos8(sineOffset * 11 + y * 4)), 2);

			leds[XY(x, y)] = CRGB(255 - sinDistanceR, 255 - sinDistanceG, 255 - sinDistanceB);
		}
	}

	sineOffset++; // byte will wrap from 255 to 0, matching sin8 0-255 cycle

}

	
void shadesOutline2() {
	static boolean erase = false;
	static uint8_t x, y, z = 0;
	static uint8_t currentColor = 0;
	//startup tasks
	if (effectInit == false) {
		effectInit = true;
		erase = false;
		x = 0;
		y = 0;
		z = 0;
		effectDelay = 15;
		FastLED.clear();
		currentPalette = RainbowColors_p;
		fadeActive = 1;
	}  
	const uint8_t OutlineTable[] = {
		0,  1,  2,  3,  4,  5,  6,  17,  18,  29, 30, 31, 32, 33, 34, 35, 24, 23, 12, 11, 0
	};
	const uint8_t OutlineTable2[] = {
	 10, 9, 8, 7, 16, 19, 28, 27, 26, 25, 22, 13, 10
	};  
	const uint8_t OutlineTable3[] = {
		14, 15, 20, 21
	};
	
	leds[OutlineTable[x]] = currentPalette[currentColor];
	if (erase)
		leds[OutlineTable[x]] = currentPalette[currentColor];
	x++;
	if (x == (sizeof(OutlineTable))) {
		erase = !erase;
		x = 0; 
		currentColor += random8(3, 6);
		 if (currentColor > 15) currentColor -= 16;
	}

	leds[OutlineTable2[y]] = currentPalette[currentColor];
	if (erase)
		leds[OutlineTable2[y]] = currentPalette[currentColor];
	y++;
	if (y == (sizeof(OutlineTable2))) {
		erase = !erase;
		y = 0;
//    currentColor += random8(3, 3);
//     if (currentColor > 6) currentColor -= 7;
	}
	
	leds[OutlineTable3[z]] = currentPalette[currentColor];
	if (erase)
		leds[OutlineTable3[z]] = currentPalette[currentColor];
	z++;
	if (z == (sizeof(OutlineTable3))) {
		erase = !erase;
		z = 0;
//    currentColor += random8(1, 1);
//    if (currentColor > 6) currentColor -= 7;
	}
}

void greenPortal() {
	static byte offset  = 0; // counter for radial color wave motion
	static int plasVector = 0; // counter for orbiting plasma center

	// startup tasks
	if (effectInit == false) {
		effectInit = true;
		effectDelay = 10;
		currentPalette = es_emerald_dragon_08_gp;
		fadeActive = 0;
	}

	int xOffset = 0;
	int yOffset = 0;

	// Draw one frame of the animation into the LED array
	for (int x = 0; x < kMatrixWidth; x++) {
		for (int y = 0; y < kMatrixHeight; y++) {
			byte color = sin8(sqrt(sq(((float)x - 2.5) * 12 + xOffset) + sq(((float)y - 2.5) * 12 + yOffset)) + offset);
			leds[XY(x, y)] = ColorFromPalette(currentPalette, color, 255);
		}
	}
	offset++; // wraps at 255 for sin8
	plasVector += 1; // using an int for slower orbit (wraps at 65536)
}

void colorPortal() {
	static byte offset  = 0; // counter for radial color wave motion
	static int plasVector = 0; // counter for orbiting plasma center

	// startup tasks
	if (effectInit == false) {
		effectInit = true;
		effectDelay = 10;
		// currentPalette = es_emerald_dragon_08_gp;
		fadeActive = 0;
	}
	
	ChangePalette();
	
	int xOffset = 0;
	int yOffset = 0;

	// Draw one frame of the animation into the LED array
	for (int x = 0; x < kMatrixWidth; x++) {
		for (int y = 0; y < kMatrixHeight; y++) {
			byte color = sin8(sqrt(sq(((float)x - 2.5) * 12 + xOffset) + sq(((float)y - 2.5) * 12 + yOffset)) + offset);
			leds[XY(x, y)] = ColorFromPalette(currentPalette, color, 255);
		}
	}
	offset++; // wraps at 255 for sin8
	plasVector += 1; // using an int for slower orbit (wraps at 65536)
}

void fireworks() {
	byte sparksDone = 0;
	static int sparkLife = 50;
	static boolean boom = false;
	
	// startup tasks
	if (effectInit == false) {
		effectInit = true;
		effectDelay = 5;
		gSkyburst = 1;
		fadeActive = 1;
	}
	if (boom) {
		FastLED.clear();
		boom = false;
	} else {
		fadeAll(40);
	}  
	if (sparkLife > 0) sparkLife--;

	for( byte b = 0; b < NUM_SPARKS; b++) {
		if (sparkLife <= 0) gSparks[b].show = 0;
		gSparks[b].Move();
		gSparks[b].Draw();
		sparksDone += gSparks[b].show;
	}

	if (sparksDone == 0) gSkyburst = 1;
	//Serial.println(sparksDone);

	if( gSkyburst) {
		effectDelay = 5;
		sparkLife = random(16,150);
		CRGB color;
		hsv2rgb_rainbow( CHSV( random8(), 255, 255), color);
		accum88 sx = random(127-64,127+64)<<8;
		accum88 sy = random(127-16,127+16)<<8;
		for( byte b = 0; b < NUM_SPARKS; b++) {
			gSparks[b].Skyburst(sx, sy, 0, color);
		}
		gSkyburst = 0;
		sparksDone = 0;
		fillAll(CRGB::Gray);
		boom = true;
	}
}

void amazingNoise() {
	if (effectInit == false) {
		effectInit = true;
		effectDelay = 25;
		FastLED.clear();
		fadeActive = 0;
	}
	ChangePaletteAndSettingsPeriodically();
	fillnoise8();
	mapNoiseToLEDsUsingPalette();
}

void BlacK_Blue_Magenta_WhiteNoise() {
	if (effectInit == false) {
		effectInit = true;
		effectDelay = 25;
		FastLED.clear();
		 currentPalette = BlacK_Blue_Magenta_White_gp;
		 speed =  15; 
		 scale = 20; 
		 colorLoop = 1;
		 fadeActive = 0;
	}
	fillnoise8();
	mapNoiseToLEDsUsingPalette();
}

void SunsetNoise() {
	if (effectInit == false) {
		effectInit = true;
		effectDelay = 25;
		FastLED.clear();
		 currentPalette = Sunset_Real_gp;
		 speed =  20; 
		 scale = 15; 
		 colorLoop = 1;
		 fadeActive = 0;
	}
	fillnoise8();
	mapNoiseToLEDsUsingPalette();
}

void es_vintage_57Noise() {
	if (effectInit == false) {
		effectInit = true;
		effectDelay = 25;
		FastLED.clear();
		 currentPalette = CloudColors_p;
		 speed =  20; 
		 scale = 110; 
		 colorLoop = 1;
		 fadeActive = 0;
	}
	fillnoise8();
	mapNoiseToLEDsUsingPalette();
}

void CloudNoise() {
	if (effectInit == false) {
		effectInit = true;
		effectDelay = 25;
		FastLED.clear();
		 currentPalette = CloudColors_p;
		 speed =  15; 
		 scale = 20; 
		 colorLoop = 1;
		 fadeActive = 0;
	}
	fillnoise8();
	mapNoiseToLEDsUsingPalette();
}

void ForestNoise() {

	if (effectInit == false) {
		effectInit = true;
		effectDelay = 25;
		FastLED.clear();
		 currentPalette = ForestColors_p;
		 speed = 8; 
		 scale = 120; 
		 colorLoop = 1;
		 fadeActive = 0;
	}
	// Periodically choose a new palette, speed, and scale
	// ChangePaletteAndSettingsPeriodically();
//    if( secondHand == 15)  { currentPalette = ForestColors_p;           }
//    if( secondHand == 20)  { currentPalette = CloudColors_p;           speed =  4; scale = 30; colorLoop = 0; }
//    if( secondHand == 25)  { currentPalette = LavaColors_p;            speed =  8; scale = 50; colorLoop = 0; }
//    if( secondHand == 30)  { currentPalette = OceanColors_p;           speed = 20; scale = 90; colorLoop = 0; }
//    if( secondHand == 35)  { currentPalette = PartyColors_p;           speed = 20; scale = 30; colorLoop = 1;
	// generate noise data
	fillnoise8();
	mapNoiseToLEDsUsingPalette();
}

uint8_t amazingAngle = 16;
void amazing() {

	if (effectInit == false) {
		effectInit = true;
		effectDelay = 10;
		FastLED.clear();
		fadeActive = 0;
		// selectRandomPalette();
		//currentPalette = retro2_16_gp;
	}
	uint8_t h = sin8(amazingAngle);
	
	for (int16_t x= kMatrixWidth; x>=0; --x) {
		for (int16_t y= kMatrixHeight; y>=0; --y) {
			leds[XY(x, y)] = CHSV(h, 255, 255);
			h += 32;
		}
	}
	amazingAngle += 4;
}

#define rainDir2 1
void rain() {

	if (effectInit == false) {
		effectInit = true;    
		effectDelay = 240;
		fadeActive = 0;
	}
	
	scrollArray(rainDir2);
	byte randPixel = random8(kMatrixHeight);
	for (byte y = 0; y < kMatrixHeight; y++) leds[XY((kMatrixWidth - 1) * rainDir2, y)] = CRGB::Black;  leds[XY((kMatrixWidth - 1)*rainDir2, randPixel)] = CHSV(cycleHue, 255, 255);
}

// Triple Sine Waves
void threeSine() {

	static byte sineOffset = 0; // counter for current position of sine waves

	// startup tasks
	if (effectInit == false) {
		effectInit = true;
		effectDelay = 20;
		fadeActive = 0;
	}

	// Draw one frame of the animation into the LED array
	for (byte x = 0; x < kMatrixWidth; x++) {
		for (int y = 0; y < kMatrixHeight; y++) {

			// Calculate "sine" waves with varying periods
			// sin8 is used for speed; cos8, quadwave8, or triwave8 would also work here
			byte sinDistanceR = qmul8(abs(y * (255 / kMatrixHeight) - sin8(sineOffset * 9 + x * 16)), 2);
			byte sinDistanceG = qmul8(abs(y * (255 / kMatrixHeight) - sin8(sineOffset * 10 + x * 16)), 2);
			byte sinDistanceB = qmul8(abs(y * (255 / kMatrixHeight) - sin8(sineOffset * 11 + x * 16)), 2);

			leds[XY(x, y)] = CRGB(255 - sinDistanceR, 255 - sinDistanceG, 255 - sinDistanceB);
		}
	}

	sineOffset++; // byte will wrap from 255 to 0, matching sin8 0-255 cycle

}


// RGB Plasma
void plasma() {

	static byte offset  = 0; // counter for radial color wave motion
	static int plasVector = 0; // counter for orbiting plasma center

	// startup tasks
	if (effectInit == false) {
		effectInit = true;
		effectDelay = 20;
		fadeActive = 0;
	}

	// Calculate current center of plasma pattern (can be offscreen)
	int xOffset = cos8(plasVector / 256);
	int yOffset = sin8(plasVector / 256);

	// Draw one frame of the animation into the LED array
	for (int x = 0; x < kMatrixWidth; x++) {
		for (int y = 0; y < kMatrixHeight; y++) {
			byte color = sin8(sqrt(sq(((float)x - 8) * 10 + xOffset - 8) + sq(((float)y - 2) * 10 + yOffset - 127)) + offset);
			leds[XY(x, y)] = CHSV(color, 255, 255);
		}
	}

	offset++; // wraps at 255 for sin8
	plasVector += 16; // using an int for slower orbit (wraps at 65536)

}


// Scanning pattern left/right, uses global hue cycle
void rider() {

	static byte riderPos = 0;

	// startup tasks
	if (effectInit == false) {
		effectInit = true;
		effectDelay = 5;
		riderPos = 0;
		fadeActive = 0;
	}

	// Draw one frame of the animation into the LED array
	for (byte x = 0; x < kMatrixWidth; x++) {
		int brightness = abs(x * (256 / kMatrixWidth) - triwave8(riderPos) * 2 + 127) * 3;
		if (brightness > 255) brightness = 255;
		brightness = 255 - brightness;
		CRGB riderColor = CHSV(cycleHue, 255, brightness);
		for (byte y = 0; y < kMatrixHeight; y++) {
			leds[XY(x, y)] = riderColor;
		}
	}

	riderPos++; // byte wraps to 0 at 255, triwave8 is also 0-255 periodic

}


// Shimmering noise, uses global hue cycle
void glitter() {

	// startup tasks
	if (effectInit == false) {
		effectInit = true;
		effectDelay = 20;
		fadeActive = 0;
	}

	// Draw one frame of the animation into the LED array
	for (int x = 0; x < kMatrixWidth; x++) {
		for (int y = 0; y < kMatrixHeight; y++) {
			leds[XY(x, y)] = CHSV(cycleHue, 255, random8(5) * 63);
		}
	}

}


// Fills saturated colors into the array from alternating directions
void colorFill() {

	static byte currentColor = 0;
	static byte currentRow = 0;
	static byte currentDirection = 0;

	// startup tasks
	if (effectInit == false) {
		effectInit = true;
		effectDelay = 45;
		currentColor = 0;
		currentRow = 0;
		currentDirection = 0;
		currentPalette = RainbowColors_p;
		fadeActive = 0;
	}

	// test a bitmask to fill up or down when currentDirection is 0 or 2 (0b00 or 0b10)
	if (!(currentDirection & 1)) {
		effectDelay = 12; // slower since vertical has fewer pixels
		for (byte x = 0; x < kMatrixWidth; x++) {
			byte y = currentRow;
			if (currentDirection == 2) y = kMatrixHeight - 1 - currentRow;
			leds[XY(x, y)] = currentPalette[currentColor];
		}
	}

	// test a bitmask to fill left or right when currentDirection is 1 or 3 (0b01 or 0b11)
	if (currentDirection & 1) {
		effectDelay = 12; // faster since horizontal has more pixels
		for (byte y = 0; y < kMatrixHeight; y++) {
			byte x = currentRow;
			if (currentDirection == 3) x = kMatrixWidth - 1 - currentRow;
			leds[XY(x, y)] = currentPalette[currentColor];
		}
	}

	currentRow++;

	// detect when a fill is complete, change color and direction
	if ((!(currentDirection & 1) && currentRow >= kMatrixHeight) || ((currentDirection & 1) && currentRow >= kMatrixWidth)) {
		currentRow = 0;
		currentColor += random8(3, 6);
		if (currentColor > 15) currentColor -= 16;
		currentDirection++;
		if (currentDirection > 3) currentDirection = 0;
		effectDelay = 300; // wait a little bit longer after completing a fill
	}


}

// Emulate 3D anaglyph glasses
void threeDee() {

	// startup tasks
	if (effectInit == false) {
		effectInit = true;
		effectDelay = 50;
		fadeActive = 0;
	}

	for (byte x = 0; x < kMatrixWidth; x++) {
		for (byte y = 0; y < kMatrixHeight; y++) {
			if (x < 3) {
				leds[XY(x, y)] = CRGB::Blue;
			} else if (x > 2) {
				leds[XY(x, y)] = CRGB::Red;
			}
		}
	}

	leds[XY(6, 0)] = CRGB::Red;
	leds[XY(9, 0)] = CRGB::Red;

}

// Random pixels scroll sideways, uses current hue
#define rainDir 0
void sideRain() {

	// startup tasks
	if (effectInit == false) {
		effectInit = true;
		effectDelay = 30;
		fadeActive = 0;
	}

	scrollArray(rainDir);
	byte randPixel = random8(kMatrixHeight);
	for (byte y = 0; y < kMatrixHeight; y++) leds[XY((kMatrixWidth - 1) * rainDir, y)] = CRGB::Black;
	leds[XY((kMatrixWidth - 1)*rainDir, randPixel)] = CHSV(cycleHue, 255, 255);

}

void juggle() {
	
	if (effectInit == false) {
		effectInit = true;
		FastLED.clear(); 
		effectDelay = 45;
		selectRandomPalette();
		fadeActive = 0;
	}
	
	fadeToBlackBy( leds, NUM_LEDS, 200);
	byte dothue = 0;
	for ( int i = 0; i < 6; i++) {
		leds[beatsin16( i + 6, 0, NUM_LEDS - 1 )] |= CHSV(dothue, 200, 255);
		dothue += 32;
	}
}

void barfight() {
	static byte barpos[6];
	
	if (effectInit == false) {
		effectInit = true;
		effectDelay = 30;
		fadeActive = 0;
		for (byte i = 0; i < kMatrixWidth; i++) {
			barpos[i] = random8(0,6);
		}
	}

	for (byte x = 0; x < kMatrixWidth; x++) {
		for (byte y = 0; y < kMatrixHeight; y++) {
			if (y < barpos[x]) {
				// leds[XY(x,y)] = CHSV(cycleHue, 255, 255);
				leds[XY(x,y)] = CRGB::Red;
				//leds[XY(x,y)] = CRGB::DarkMagenta;
			} else {
				// leds[XY(x,y)] = CRGB(0,128,0);
				leds[XY(x,y)] = CRGB::Yellow;
			}
		}

		byte tempIncr = random(0,6);
		if (barpos[x] > 0 && tempIncr == 0) barpos[x]--;
		if (barpos[x] < 16 && tempIncr == 2) barpos[x]++;

		
	}

}

void confetti() {

	// startup tasks
	if (effectInit == false) {
		effectInit = true;
		effectDelay = 10;
		selectRandomPalette();
		fadeActive = 0;
	}

	// scatter random colored pixels at several random coordinates
	for (byte i = 0; i < 4; i++) {
		leds[XY(random16(kMatrixWidth), random16(kMatrixHeight))] = ColorFromPalette(currentPalette, random16(255), 255); //CHSV(random16(255), 255, 255);
		random16_add_entropy(1);
	}

}


// Draw slanting bars scrolling across the array, uses current hue
void slantBars() {

	static byte slantPos = 0;

	// startup tasks
	if (effectInit == false) {
		effectInit = true;
		effectDelay = 5;
		fadeActive = 0;
	}

	for (byte x = 0; x < kMatrixWidth; x++) {
		for (byte y = 0; y < kMatrixHeight; y++) {
			leds[XY(x, y)] = CHSV(cycleHue, 255, quadwave8(x * 32 + y * 32 + slantPos));
		}
	}

	slantPos -= 4;

}


//leds run around the periphery of the shades, changing color every go 'round
void shadesOutline() {
	
	static uint8_t x = 0;
	
	//startup tasks
	if (effectInit == false) {
		effectInit = true;
		effectDelay = 15;
		FastLED.clear();
		currentPalette = RainbowColors_p;
		fadeActive = 1;
	}

	CRGB pixelColor = CHSV(cycleHue, 255, 255);
	leds[OutlineMap(x)] = pixelColor;

	x++;
	if (x > (OUTLINESIZE-1)) x = 0;
	
}

//leds run around the periphery of the shades, changing color every go 'round
void spirals() {
	
	static uint8_t x = 0;
	
	//startup tasks
	if (effectInit == false) {
		effectInit = true;
		effectDelay = 10;
		FastLED.clear();
		currentPalette = RainbowColors_p;
		fadeActive = 1;
	}

	CRGB pixelColor = CHSV(cycleHue, 255, 255);
	leds[OutlineMap2(x)] = pixelColor;

	x++;
	if (x > (OUTLINESIZE2-1)) x = 0;
	
}

#define NORMAL 0
#define RAINBOW 1
#define charSpacing 2
// Scroll a text string
void scrollText(byte message, byte style, CRGB fgColor, CRGB bgColor) {
	static byte currentMessageChar = 0;
	static byte currentCharColumn = 0;
	static byte paletteCycle = 0;
	static CRGB currentColor;
	static byte bitBuffer[6] = {0};
	static byte bitBufferPointer = 0;

	// startup tasks
	if (effectInit == false) {
		effectInit = true;
		effectDelay = 35;
		fadeActive = 0;
		currentMessageChar = 0;
		currentCharColumn = 0;
		selectFlashString(message);
		loadCharBuffer(loadStringChar(message, currentMessageChar));
		currentPalette = RainbowColors_p;
		for (byte i = 0; i < kMatrixWidth; i++) bitBuffer[i] = 0;
	}

	paletteCycle += 5;

	if (currentCharColumn < 5) { // characters are 5 pixels wide
		bitBuffer[(bitBufferPointer + kMatrixWidth - 1) % kMatrixWidth] = charBuffer[currentCharColumn]; // character
	} else {
		bitBuffer[(bitBufferPointer + kMatrixWidth - 1) % kMatrixWidth] = 0; // space
	}

	CRGB pixelColor;
	for (byte x = 0; x < kMatrixWidth; x++) {
		for (byte y = 0; y < 6; y++) { // characters are 5 pixels tall
			if (bitRead(bitBuffer[(bitBufferPointer + x) % kMatrixWidth], y) == 1) {
				if (style == RAINBOW) {
					pixelColor = ColorFromPalette(currentPalette, paletteCycle+y*16, 255);
				} else {
					pixelColor = fgColor;
				}
			} else {
				pixelColor = bgColor;
			}
			leds[XY(x, y)] = pixelColor;
		}
	}

	currentCharColumn++;
	if (currentCharColumn > (6 + charSpacing)) {
		currentCharColumn = 0;
		currentMessageChar++;
		char nextChar = loadStringChar(message, currentMessageChar);
		if (nextChar == 0) { // null character at end of strong
			currentMessageChar = 0;
			nextChar = loadStringChar(message, currentMessageChar);
		}
		loadCharBuffer(nextChar);
	}

	bitBufferPointer++;
	if (bitBufferPointer > 5) bitBufferPointer = 0;

}


void scrollTextZero() {
	scrollText(0, NORMAL, CRGB::Red, CRGB::Black);
}

void scrollTextOne() {
	scrollText(1, RAINBOW, 0, CRGB::Black);
}

void scrollTextTwo() {
	scrollText(2, NORMAL, CRGB::Green, CRGB(0,0,8));
}
