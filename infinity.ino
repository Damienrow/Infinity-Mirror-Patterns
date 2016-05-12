#include <Adafruit_NeoPixel.h>

/****************************************
Various led strip patterns. Also Some button controls
to switch between the patterns.
****************************************/

// leds
#define LED_PIN 4
#define NUMPIXELS 30
#define PI 3.1415

const int buttonPin1 = 10;     // the number of the pushbutton pin
const int buttonPin2 = 9;     // the number of the pushbutton pin
const int buttonPin3 = 8;     // the number of the pushbutton pin

// button vars
bool buttonDown = false;
bool buttonPressed = false;
int buttonState = 0;         // variable for reading the pushbutton status

// mode vars
int curMode = 0; // the current mode
int curAltMode = 0; // the alt setting for the current mode
int numModes = 15;
int numHolidayModes = 5;

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);

// rgb vars
uint8_t rgbColor[3]; 
uint8_t decColor;
uint8_t incColor;

// mic
const int sampleWindow = 50; // Sample window width in mS (50 mS = 20Hz)
unsigned int sample;
float vcc = 3.3;

//float mic_dc = vcc/2;
float peak_to_peak_dc = 0.1;
float min_mic_volts = 0;
float max_mic_volts = 2.37;

// pulse wave vars
const uint8_t animStartLEDNum = 0;
const float pulseFadeRate = 0.57;
const uint8_t pulseTailLength = 7;
const uint8_t pulseSpeed = 55; // inverse, lower numbers are faster

// twinkle vars
const float twinkleFadeRate = 0.96;
const int twinkleSpawnRate = 20;

// breathe vars
int breatheState;

// colored_breathe vars
uint32_t breatheColor;

// rainbow cycle vars
int rainbowCycleState = 0;

// theater chase vars
int theaterChaseState = 0;

// running lights vars
int running_lights_state = 0;

// boucning ball vars
// Note: the length of all arrays must be the same as ballCount
const int ballCount = 1;
const float Gravity = -9.81;
const int StartHeight = 1;

float Height[1];
float ImpactVelocityStart = sqrt( -2 * Gravity * StartHeight );
float ImpactVelocity[1];
float TimeSinceLastBounce[1];
int   Position[1];
long  ClockTimeSinceLastBounce[1];
float Dampening[1];

// lightning vars
const int lightning_sec_count = 9;  // the maximum "width" of each lightning sequence
const int lightning_seq_count = 9;  // the maximum "duration" of each lightning sequence

// new years vars
int newYearsFireworksCounter = -1;
int newYearsFireworksState = 0;
int newYearsFireworksTwinkleFadeState = 0;
int newYearsFireworksTwinkleSpreadState = 0;
int newYearsFireworksStartPoint;
int newYearsFireworksEndPoint;
int newYearsFireworksDir;

// valentines vars
int valentinesState = 0;
uint32_t curValentinesColor;

// Thanksgiving vars
int thanksgivingState = 0;
uint32_t curColor;
int thanksgivingWaitState = 0;

// Holiday colors
// christmas palette
uint32_t christmas_red;
uint32_t christmas_green;
uint32_t christmas_gold;
uint32_t christmas_blue;
uint32_t christmas_pink;

// fourth of july palette
uint32_t fourth_red;
uint32_t fourth_white;
uint32_t fourth_blue;

// new years palette
uint32_t new_years_fireworks_yellow;

// valentine's day palette
uint32_t valentines_pink;
uint32_t valentines_red;
uint32_t valentines_purple;

// Thanksgiving color palette
uint32_t thanksgiving_brown;
uint32_t thanksgiving_lightbrown;
uint32_t thanksgiving_red;

// Halloween color pallette
uint32_t halloween_green;
uint32_t halloween_brown; // combination of green and purple
uint32_t halloween_purple;

void setup() 
{
   // bouncing ball setup
   for (int i = 0 ; i < ballCount ; i++) {   
        ClockTimeSinceLastBounce[i] = millis();
        Height[i] = StartHeight;
        Position[i] = 0; 
        ImpactVelocity[i] = ImpactVelocityStart;
        TimeSinceLastBounce[i] = 0;
        Dampening[i] = 0.90 - float(i)/pow(ballCount,2); 
   }
    
   // set up fourth of july palette
   fourth_red = pixels.Color(141, 1, 4);
   fourth_white = pixels.Color(255, 255, 255);
   fourth_blue = pixels.Color(15, 0, 103);

   // christmas palette setup
   christmas_red = pixels.Color(204, 0, 0);
   christmas_green = pixels.Color(0, 102, 0);
   christmas_gold = pixels.Color(218, 165, 32);
   christmas_blue = pixels.Color(0, 51, 153);
   christmas_pink = pixels.Color(255, 51, 204);

   // new years color setup
   new_years_fireworks_yellow = pixels.Color(255 , 230, 63);

   // valentines day setup
   valentines_pink = pixels.Color(220, 68,  68);
   valentines_red = pixels.Color(187, 10, 10);
   valentines_purple = pixels.Color(101, 1, 92);

   // thanksgiving colors
   thanksgiving_brown = pixels.Color(79, 33, 0);
   thanksgiving_lightbrown = pixels.Color(168, 86, 12);
   thanksgiving_red = pixels.Color(112, 9, 0);

   // halloween colors
   halloween_green = pixels.Color(74, 150, 12);
   halloween_brown = pixels.Color(128, 91, 86);
   halloween_purple = pixels.Color(158, 8, 148);
   
   // set up rgb incrementer
   decColor = 0; // Start off with red. 
   incColor = decColor == 2 ? 0 : decColor + 1;
   rgbColor[decColor] = 255;
   rgbColor[(decColor + 1) % 3] = 0;
   rgbColor[(decColor + 2) % 3] = 0;

   breatheColor = pixels.Color(random(255), random(255), random(255));

   randomSeed(analogRead(5));

   pinMode(buttonPin1, INPUT);
   pinMode(buttonPin2, INPUT);
   pinMode(buttonPin3, INPUT);
   
   pixels.begin(); 
   Serial.begin(9600);
}
 
 
void loop() 
{
   // music_visualizer();
   // pulse_wave(pixels.Color(255, 255, 210), pulseTailLength, pulseFadeRate, pulseSpeed); // shooting star light with tail
   // pulse_wave(pixels.Color(random(255), random(255), random(255)), 10, 0.80, 70); // random colored light with tail
   // fire_pulse();
   // twinkle(pixels.Color(255, 255, 210), twinkleSpawnRate); // start twinkle
   // twinkle(pixels.Color(random(255), random(255), random(255)), twinkleSpawnRate); // random color twinkle
   // breathe(pixels.Color(255, 255, 210), 200, 0.009, 5);
   // breathe(pixels.Color(random(255), random(255), random(255)), 200, 0.009, 5);
   // star_fall(pixels.Color(255, 255, 210), pulseTailLength, pulseFadeRate, 10, 60);

//   buttonControl();
//   switch(curMode){
//        case 0:
//            music_visualizer();
//            break;
//        case 1:
//            twinkle(pixels.Color(random(255), random(255), random(255)), twinkleSpawnRate); // random color twinkle      
//            break;
//        case 2:
//            twinkle(pixels.Color(255, 255, 210), twinkleSpawnRate); // start twinkle         
//            break;
//        case 3:
//            pulse_wave(pixels.Color(255, 255, 210), pulseTailLength, pulseFadeRate, pulseSpeed); // shooting star light with tail
//            break;
//        case 4:
//            star_fall(pixels.Color(255, 255, 210), pulseTailLength, pulseFadeRate, 8, 60);
//            break;
//        case 5:
//            breathe(pixels.Color(255, 255, 210), 200, 0.009, 5);
//            break;
//        case 6:
//            colored_breathe(200, 0.009, 5);
//            break;
//        case 7:
//            rainbow_cycle(20);
//            break;
//        case 8:
//            theater_chase(pixels.Color(127, 127, 127), 50); // White
//            break;      
//        case 9:
//            christmas_twinkle(true);
//            break;      
//        case 10:
//            fourth_of_july_fireworks(pulseTailLength, pulseFadeRate, 8, 60);
//            break;   
//        case 11:
//            bouncing_balls(pixels.Color(255, 91, 71), 1);
//            break;   
//        case 12:
//            fire(55, 120, 50);
//            break;
//        case 13:
//            lightning(5, 6);
//            break;
//        case 14:
//            thanksgiving_chase(10, 100);
//            break;
//   }
//      strobe(pixels.Color(255, 255, 255), 100);
//      sparkle(pixels.Color(175, 195, 255), 10);
//      snow_sparkle(pixels.Color(40, 40, 40), 70, 100, 100);
//      running_lights(0xff,0xff,0x28, 125);
//      halloween_fire();
      new_years_fireworks();
//      valentines_breathe();
     
}

bool buttonControl(){
    // check if button pressed (down and release == a press)
    if(digitalRead(buttonPin1) == HIGH)
    {
        buttonDown = true;
    }else{
        if(buttonDown){
            buttonPressed = true;
            buttonDown = false;
        }
    }
  
    if (buttonPressed) {
        curMode = (1 + curMode) % numModes;
        buttonPressed = false;
    }
}

/************************
 *                      *
 *   Holiday Patterns   *
 *                      *
 ************************/

// thanksgiving colors move along the led strip
void thanksgiving_chase(int waitMultiplyer, uint8_t wait) {
    thanksgivingWaitState++;
    if(thanksgivingWaitState % waitMultiplyer != 0){
        delay(wait);
        return;
    }
    
    thanksgivingState %= NUMPIXELS;

    curColor = thanksgiving_brown;

    // the second half of the pattern
    int i;
    for (i = 0; i + thanksgivingState < NUMPIXELS; i++) {
        pixels.setPixelColor(i + thanksgivingState, curColor);
        if((i + 1) % 3 == 0){
            if(curColor == thanksgiving_lightbrown){
                curColor = thanksgiving_brown;
            }else if(curColor == thanksgiving_brown){
                curColor = thanksgiving_red;
            }else{
                curColor = thanksgiving_lightbrown;
            }
        }
    }

    // the first half of the pattern
    for (int j = 0; j  < thanksgivingState; j++) {
        pixels.setPixelColor(j, curColor);
        if((j + i + 1) % 3 == 0){
            if(curColor == thanksgiving_lightbrown){
                curColor = thanksgiving_brown;
            }else if(curColor == thanksgiving_brown){
                curColor = thanksgiving_red;
            }else{
                curColor = thanksgiving_lightbrown;
            }
        }
    }
    
    pixels.show();
    delay(wait);
    thanksgivingState += 1;
}

// simple "breathing" (dim and brighten) effect with Valentine's colors
void valentines_breathe(){
    float breatheSpeed = 0.01; 
    int breatheDelay = 5;
    int maxBrightness = 200;

    if(valentinesState == 0){
        clear_leds();
        switch(random(3)){
            case 0:
                curValentinesColor = valentines_pink;
                break;
            case 1:
                curValentinesColor = valentines_red;
                break;
            case 2:
                curValentinesColor = valentines_purple;
                break;
        }
    }
    
    int breathePeriod = 2 * PI / breatheSpeed;  
    
    valentinesState++;
    if(valentinesState == breathePeriod){
        valentinesState = 0;
    }
      
    float intensity = (maxBrightness / 2.0) * (1.0 + sin(breatheSpeed * valentinesState - PI/2));
    set_color(curValentinesColor);
    setBrightness(intensity);
    pixels.show();
    delay(breatheDelay);
}

// Randomly spawn a pulse wave that travels to a spot near the center of the strip and
// "explodes" outward in a twinkling pattern that slowly fades (e.g. a firework).
// If the timeing of this function looks like nonsense, that's because it is.
// The timing was hand coded by trial and error and will need to be adjusted the same way.
void new_years_fireworks(){
  // spawn a new firework
	if(newYearsFireworksCounter == -1){
    // set the starting point
		switch(random(2) == 0){
			case 0:
				newYearsFireworksStartPoint = 0;
				newYearsFireworksDir = -1;
				break;
			case 1:
				newYearsFireworksStartPoint = NUMPIXELS - 1;
				newYearsFireworksDir = 1;
				break;
  		}

     // set the ending point
      newYearsFireworksEndPoint = random(NUMPIXELS * 3 / 8, NUMPIXELS * 5 / 8);

      // set up other variables
      newYearsFireworksCounter++;

      // spawn the new firework
  		pixels.setPixelColor(newYearsFireworksStartPoint, new_years_fireworks_yellow);
	} // end spawn a new firework

	switch(newYearsFireworksCounter){
		case 0:
        // play out the rising pulse wave
  			new_years_fireworks_rising(new_years_fireworks_yellow, 5, 0.57, 
      			newYearsFireworksStartPoint, newYearsFireworksEndPoint, newYearsFireworksDir);
        if(new_years_fireworks_finished_current()){
            newYearsFireworksCounter++;
            newYearsFireworksState = 0;
        }
  			break;
		case 1:
        // these numbers are both arbitrary. Just need to stall for a few seconds to create the firework effect
        delay(10); 
        if(newYearsFireworksState++ == 100){
            newYearsFireworksCounter++;
            newYearsFireworksTwinkleFadeState = 255;
            newYearsFireworksTwinkleSpreadState = 0;
        }
        break;
    case 2:
        // play out the twinlking fading effect
        new_years_fireworks_twinkle(newYearsFireworksEndPoint);
        if(newYearsFireworksTwinkleFadeState == 0){
            newYearsFireworksCounter++;
            newYearsFireworksState = 0;
        }
			  break;
		case 3:
        // random pause before the next firework spawns
        if(random(150) == 1){
            newYearsFireworksCounter = -1;
        }
        delay(10); 
			  break;
	}
}

bool new_years_fireworks_finished_current(){
    for(int i = 0; i < NUMPIXELS; i++){
        if(pixels.getPixelColor(i) != 0){
            return false;
        }
    }
    return true;
}

void new_years_fireworks_twinkle(int center){
    // scale the number of twinkles
    //       (b-a)(x - min)
    //f(x) = --------------  + a
    //          max - min
    int twinkleScale = NUMPIXELS / 9 - (((NUMPIXELS / 9 - 1) * (newYearsFireworksTwinkleFadeState))/(255) + 1);
    Serial.println(twinkleScale);
    
    for(int j = 0; j < NUMPIXELS / 9 - twinkleScale; j++){
        int i = random(center - newYearsFireworksTwinkleSpreadState, center + newYearsFireworksTwinkleSpreadState + 1);
        pixels.setPixelColor(i, new_years_fireworks_yellow);   
    }
    setBrightness(newYearsFireworksTwinkleFadeState);
    pixels.show();
    delay(10);
    newYearsFireworksTwinkleFadeState--;

    // spread out the sparkle range
    if(newYearsFireworksTwinkleSpreadState < NUMPIXELS / 4){
        newYearsFireworksTwinkleSpreadState++;
    }
    
    clear_leds();
}

// dir must be
void new_years_fireworks_rising(uint32_t headColor, int tailLength, float fadeRate, int startPoint, int endPoint, int dir){
	// move the pulse along
    for(int i = endPoint; i != startPoint; i += dir){
        pixels.setPixelColor(i, pixels.getPixelColor(i + dir));
    }


    // spawn more of the pulse tail
    int firstHeadPos = star_fall_find_head(headColor, startPoint, endPoint, -1 * dir); 
    int headDist = (dir == 1) ? (endPoint - firstHeadPos) : (firstHeadPos - endPoint);
    if(firstHeadPos != -1 && headDist < tailLength + 1){
        if(headDist < tailLength){
            pixels.setPixelColor(startPoint, pixels.Color( getPixelColorComponent(startPoint + -dir, 1) * fadeRate,
                                                       getPixelColorComponent(startPoint + -dir, 2) * fadeRate,
                                                       getPixelColorComponent(startPoint + -dir, 3) * fadeRate));
        }else{
            pixels.setPixelColor(startPoint, pixels.Color( 0, 0, 0 ));
        }
        // pixels.setPixelColor(center - 1, pixels.getPixelColor(center));                                           
    }

    pixels.show();
    delay(90);
}

// randomly briefly light up sections of the led strip in a way that is evocative of lighting
void lightning(int num_sections, int section_len)
{
    // randomly select the color of the current lightning sequence
    int r = random(200, 255);
    int g = random(150, 255) - 100;
    int b = random(200, 255);
  
    // randomly select the order in which the LED sections will flash
    // for the current lightning sequence
    int seq_order[lightning_seq_count];
    int seq_max = 0;
    for (int i = 0; i < lightning_seq_count; i++)
    {
        seq_order[i] = random(0, lightning_sec_count);
        seq_max = max(seq_max, seq_order[i]);
    }
    
    // randomly select the "duration" of the current lightning sequence
    int sections = random(5, lightning_seq_count);
    
    // randomly select the starting location of the current lightning sequence
    int seq_start = random(0, num_sections - seq_max);
    
    // loop through each of the chosen sections
    for (int j = 0; j < sections; j++)
    {
        // loop through each LED of the current section
        if(random(3) == 1){
            for (int k = 0; k < section_len; k++)
            {
                // turn on the current LED. Half left unlight to make a more unpredictable lighting effect
                if(random(2) == 1){
                    int pix_cur = ((seq_start + seq_order[j]) * section_len) + k;
                    pixels.setPixelColor(pix_cur, pixels.Color(r, g, b));
                }
            }
        }

        // turn off the LEDs of the previous section
        for (int k = 0; k < section_len; k++)
        {
            if (j > 0)
            {
              int pix_prv = ((seq_start + seq_order[j - 1]) * section_len) + k;
              pixels.setPixelColor(pix_prv, pixels.Color(0, 0, 0));
            }
        }
    
        // very short (random) delay so we actually see the lightning
        delay(random(15, 40));
        pixels.show();
    }
    clear_leds();
}

// purple light which flickers and randomly has 1/10 green and red/brown pixels distributed thoughout.
// It is supposed to be evocative of a purple and green fire.
void halloween_fire(){
    uint8_t r, g, b;
    //  Flicker, based on our initial RGB values
    for(int i = 0; i < pixels.numPixels(); i++) {
        switch(random(10)){
            case 0:
                r = getColorComponent(halloween_green, 1);
                g = getColorComponent(halloween_green, 2);
                b = getColorComponent(halloween_green, 3);
                break;
            case 1:
                r = getColorComponent(halloween_brown, 1);
                g = getColorComponent(halloween_brown, 2);
                b = getColorComponent(halloween_brown, 3);
                break;
             default:
                r = getColorComponent(halloween_purple, 1);
                g = getColorComponent(halloween_purple, 2);
                b = getColorComponent(halloween_purple, 3);
                break;
        }
        
        int flicker = random(0, 100);
        int r1 = r - flicker;
        int g1 = g - flicker;
        int b1 = b - flicker;
        if(g1 < 0) g1 = 0;
        if(r1 < 0) r1 = 0;
        if(b1 < 0) b1 = 0;
        pixels.setPixelColor(i, r1, g1, b1);
    }
    pixels.show();
    
    //  Adjust the delay here, if you'd like.  Right now, it randomizes the 
    //  color switch delay to give a sense of realism
    delay(random(50, 150));
}

// Pulse waves are randomly spawned from the center of the strip and travel outward (firworks)
// The pulses are randomly colored red, white, or blue.
void fourth_of_july_fireworks(uint8_t tailLength, double fadeRate, int spawnRate, uint8_t wait){
    if(NUMPIXELS % 2 == 1){
        fourth_of_july_fireworks_odd(tailLength, spawnRate, fadeRate);
    }else{
        fourth_of_july_fireworks_even(tailLength, spawnRate, fadeRate);
    }
    pixels.show();
    delay(wait);
}

// checks the strip for in progress star fall patterns and moves them forward
void fourth_of_july_fireworks_odd(uint32_t tailLength, int spawnRate, double fadeRate){
    // choose direction for a new star to go
    int center = NUMPIXELS / 2;
    if(pixels.getPixelColor(center) == fourth_red || 
          pixels.getPixelColor(center) == fourth_white || 
          pixels.getPixelColor(center) == fourth_blue){
            
        uint32_t headColor = pixels.getPixelColor(center);
        if(random(2) == 1){
           pixels.setPixelColor(center + 1, headColor);
        }else{
           pixels.setPixelColor(center - 1, headColor);
        }

        if(tailLength > 0){
            pixels.setPixelColor(center, pixels.Color( getPixelColorComponent(center, 1) * fadeRate,
                                                       getPixelColorComponent(center, 2) * fadeRate,
                                                       getPixelColorComponent(center, 3) * fadeRate));
        }
    }
    
    // move the beginning side pattern along
    int endPos = center - 1;
    for(int i = 0; i < endPos; i++){
        pixels.setPixelColor(i, pixels.getPixelColor(i + 1));
    }
    
    // move the ending side pattern along
    endPos = center + 1;
    for(int i = NUMPIXELS - 1; i > endPos; i--){
        pixels.setPixelColor(i, pixels.getPixelColor(i - 1));
    }

    //spawn more beginning side tail (if it exists)
    int firstHeadPos = find_first_fourth_head(center - 1, center - 1, -1, -1);;
    if(firstHeadPos != -1 && (center - firstHeadPos) < tailLength + 1){
        if((center - firstHeadPos) < tailLength){
            pixels.setPixelColor(center, pixels.Color( getPixelColorComponent(center, 1) * fadeRate,
                                                      getPixelColorComponent(center, 2) * fadeRate,
                                                       getPixelColorComponent(center, 3) * fadeRate));
        }else{
            pixels.setPixelColor(center, pixels.Color( 0, 0, 0 ));
        }

        pixels.setPixelColor(center - 1, pixels.getPixelColor(center));                                           
    }
    
    //spawn more ending side tail (if it exists)
    firstHeadPos = find_first_fourth_head(center + 1, center + 1, NUMPIXELS, 1);
    if(firstHeadPos != -1 && (firstHeadPos - center) < tailLength + 1){
        if((firstHeadPos - center) < tailLength){
            pixels.setPixelColor(center, pixels.Color( getPixelColorComponent(center, 1) * fadeRate,
                                                      getPixelColorComponent(center, 2) * fadeRate,
                                                       getPixelColorComponent(center, 3) * fadeRate));
        }else{
            pixels.setPixelColor(center, pixels.Color( 0, 0, 0 ));
        }
          pixels.setPixelColor(center + 1, pixels.getPixelColor(center));   
    }

    // spawn a new star fall
    if (random(spawnRate) == 1) {
        switch(random(3)){
            case 0:
                pixels.setPixelColor(NUMPIXELS / 2, fourth_red);
                break;  
            case 1:
                pixels.setPixelColor(NUMPIXELS / 2, fourth_white);
                break;  
            case 2:
                pixels.setPixelColor(NUMPIXELS / 2, fourth_blue);
                break;  
        }
    }
}

void fourth_of_july_fireworks_even(uint32_t tailLength, int spawnRate, double fadeRate){
    int beginCenter = NUMPIXELS / 2 - 1;
    int endCenter = NUMPIXELS / 2;
    
    // move the beginning side pattern along
    for(int i = 0; i < beginCenter; i++){
        pixels.setPixelColor(i, pixels.getPixelColor(i + 1));
    }

    // move the ending side pattern along
    for(int i = NUMPIXELS; i > endCenter; i--){
        pixels.setPixelColor(i, pixels.getPixelColor(i - 1));
    }

    // spawn more beginning side tail (if it exists)
    int firstHeadPos = find_first_fourth_head(beginCenter, beginCenter, -1, -1);
    if(firstHeadPos != -1 && (beginCenter - firstHeadPos) < tailLength + 1){
        if((beginCenter - firstHeadPos) < tailLength){
            pixels.setPixelColor(beginCenter, pixels.Color( getPixelColorComponent(beginCenter, 1) * fadeRate,
                                                            getPixelColorComponent(beginCenter, 2) * fadeRate,
                                                            getPixelColorComponent(beginCenter, 3) * fadeRate));
        }else{
            pixels.setPixelColor(beginCenter, pixels.Color( 0, 0, 0 ));
        }
    }
    
    // spawn more ending side tail (if it exists)
    firstHeadPos = find_first_fourth_head(endCenter, endCenter, NUMPIXELS, 1);
    if(firstHeadPos != -1 && (firstHeadPos - endCenter) < tailLength + 1){
        if((firstHeadPos - endCenter) < tailLength){
            pixels.setPixelColor(endCenter, pixels.Color( getPixelColorComponent(endCenter, 1) * fadeRate,
                                                       getPixelColorComponent(endCenter, 2) * fadeRate,
                                                       getPixelColorComponent(endCenter, 3) * fadeRate));
        }else{
            pixels.setPixelColor(endCenter, pixels.Color( 0, 0, 0 ));
        }
    }

    // spawn a new star fall
    if (random(spawnRate) == 1) {
        uint16_t i = random(beginCenter, endCenter + 1);
        switch(random(3)){
            case 0:
                pixels.setPixelColor(i, fourth_red); 
                break;
            case 1:
                pixels.setPixelColor(i, fourth_white); 
                break;
            case 2:
                pixels.setPixelColor(i, fourth_blue); 
                break;
        }
    }
}

int find_first_fourth_head(int center, int searchStart, int searchEnd, int searchDir){
    int firstHeadPosRed = star_fall_find_head(fourth_red, searchStart, searchEnd, searchDir); 
    int firstHeadPosWhite = star_fall_find_head(fourth_white, searchStart, searchEnd, searchDir); 
    int firstHeadPosBlue = star_fall_find_head(fourth_blue, searchStart, searchEnd, searchDir); 

    // if there are no heads return -1
    if(firstHeadPosRed == -1 && firstHeadPosWhite == -1 && firstHeadPosBlue == -1){
        return -1;
    }

    // get all possble heads
    int numHeads = 0;
    int possibleHeads[3];
    if(firstHeadPosRed != -1){
        numHeads++;
        possibleHeads[numHeads - 1] = firstHeadPosRed;
    }
    if(firstHeadPosBlue != -1){
        numHeads++;
        possibleHeads[numHeads - 1] = firstHeadPosBlue;
    }
    if(firstHeadPosWhite != -1){
        numHeads++;
        possibleHeads[numHeads - 1] = firstHeadPosWhite;
    }

    int firstHeadPos;
    if (searchDir == -1){
        // find the max (closest to center)
        firstHeadPos = -1;
        for(int i  = 0; i < numHeads; i++){
            if(possibleHeads[i] > firstHeadPos){
                firstHeadPos = possibleHeads[i];
            }
        }
    }else{
        // find the min (closest to center)
        firstHeadPos = NUMPIXELS;
        for(int i  = 0; i < numHeads; i++){
            if(possibleHeads[i] < firstHeadPos){
                firstHeadPos = possibleHeads[i];
            }
        }
    }
    return firstHeadPos;
}

// Twinkle effect with christmas colors.
void christmas_twinkle(bool traditionalColors){
    // fade out
    float fadeRate = 0.97;
    for(uint16_t i = 0; i < NUMPIXELS; i++){
       if(getPixelColorComponent(i, 1) > 1 || getPixelColorComponent(i, 2) > 1 || getPixelColorComponent(i, 3) > 1){
          // randomize the fade rate
          float randFadeRate;
          float randAdder = random(3) / ((float)100);
          if(random(2) == 1){
              randFadeRate = fadeRate + randAdder;
          }else{
              randFadeRate = fadeRate - randAdder;
          }
          
          pixels.setPixelColor(i, pixels.Color( getPixelColorComponent(i,1) * randFadeRate,
                                                getPixelColorComponent(i,2) * randFadeRate,
                                                getPixelColorComponent(i,3) * randFadeRate));
        }else{
          pixels.setPixelColor(i, pixels.Color(0, 0, 0));
        }
    }

    // Spawn new colors.
    if(traditionalColors){
        // Traditional colors. Red and green appear twice as much as gold.
        for(int i = 0; i < NUMPIXELS; i++){
            if(getPixelColorComponent(i, 1) == 0 && getPixelColorComponent(i, 2) == 0 && getPixelColorComponent(i, 3) == 0){
                switch (random(5)){
                    case 0:
                        pixels.setPixelColor(i, christmas_gold);
                        break;
                    case 1:
                    case 2:
                        pixels.setPixelColor(i, christmas_green);
                        break;
                    case 3:
                    case 4:
                        pixels.setPixelColor(i, christmas_red);
                        break;
                }
            }
        }
    }else{
        // More modern colors. Add in blue and violet/pink with the more traditional green, red, and gold. 
        // Red and green appear twice as often as the other colors combined.
        for(int i = 0; i < NUMPIXELS; i++){
          if(getPixelColorComponent(i, 1) == 0 && getPixelColorComponent(i, 2) == 0 && getPixelColorComponent(i, 3) == 0){
              switch (random(9)){
                  case 0:
                      pixels.setPixelColor(i, christmas_blue);
                      break;
                  case 1:
                      pixels.setPixelColor(i, christmas_gold);
                      break;
                  case 2:
                      pixels.setPixelColor(i, christmas_pink);
                      break;
                  case 3:
                  case 4:
                  case 5:
                      pixels.setPixelColor(i, christmas_green);
                      break;
                  case 6:
                  case 7:
                  case 8:
                      pixels.setPixelColor(i, christmas_red);
                      break;
              }
          }
      }
    }
    pixels.show();
    delay(20);
}

/************************
 *                      *
 *    Misc  Patterns    *
 *                      *
 ************************/

// realistically moves a pixel up and down the strip in a simulation of a boucing ball
void bouncing_balls(uint32_t ballColor, int BallCount) {
      for (int i = 0 ; i < BallCount ; i++) {
          TimeSinceLastBounce[i] =  millis() - ClockTimeSinceLastBounce[i];
          Height[i] = 0.5 * Gravity * pow( TimeSinceLastBounce[i]/1000 , 2.0 ) + ImpactVelocity[i] * TimeSinceLastBounce[i]/1000;
    
        if ( Height[i] < 0 ) {                      
            Height[i] = 0;
            ImpactVelocity[i] = Dampening[i] * ImpactVelocity[i];
            ClockTimeSinceLastBounce[i] = millis();
      
            if ( ImpactVelocity[i] < 0.01 ) {
                ImpactVelocity[i] = ImpactVelocityStart;
            }
          }
          Position[i] = round( Height[i] * (NUMPIXELS - 1) / StartHeight);
      }
    
      for (int i = 0 ; i < BallCount ; i++) {
          pixels.setPixelColor(Position[i], ballColor);
      }
      
      pixels.show();
      clear_leds();
}

// a pillar of fire moving up the led strip. 
// Looks very good with a diffusion filter over the lights (e.g. toilet paper)
void fire(int cooling, int sparking, int speedDelay) {
  static byte heat[NUMPIXELS];
  int cooldown;
  
  // Step 1.  Cool down every cell a little
  for( int i = 0; i < NUMPIXELS; i++) {
    cooldown = random(0, ((cooling * 10) / NUMPIXELS) + 2);
    
    if(cooldown>heat[i]) {
      heat[i]=0;
    } else {
      heat[i]=heat[i]-cooldown;
    }
  }
  
  // Step 2.  Heat from each cell drifts 'up' and diffuses a little
  for( int k= NUMPIXELS - 1; k >= 2; k--) {
    heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2]) / 3;
  }
    
  // Step 3.  Randomly ignite new 'sparks' near the bottom
  if( random(255) < sparking ) {
    int y = random(7);
    heat[y] = heat[y] + random(160,255);
    //heat[y] = random(160,255);
  }

  // Step 4.  Convert heat to LED colors
  for( int j = 0; j < NUMPIXELS; j++) {
    set_pixel_heat_color(j, heat[j] );
  }

  pixels.show();
  delay(speedDelay);
}

void set_pixel_heat_color (int Pixel, byte temperature) {
    // Scale 'heat' down from 0-255 to 0-191
    byte t192 = round((temperature/255.0)*191);
    
    // calculate ramp up from
    byte heatramp = t192 & 0x3F; // 0..63
    heatramp <<= 2; // scale up to 0..252
    
    // figure out which third of the spectrum we're in:
    if( t192 > 0x80) {                     // hottest
      pixels.setPixelColor(Pixel, 255, 255, heatramp);
    } else if( t192 > 0x40 ) {             // middle
      pixels.setPixelColor(Pixel, 255, heatramp, 0);
    } else {                               // coolest
      pixels.setPixelColor(Pixel, heatramp, 0, 0);
    }
}

// moves several strips of color arround the led strip
void running_lights(byte red, byte green, byte blue, int WaveDelay) {
    running_lights_state++; // = 0; //Position + Rate;
    running_lights_state %= 100;
    
    for(int i=0; i<NUMPIXELS; i++) {
      // sine wave, 3 offset waves make a rainbow!
      //float level = sin(i+Position) * 127 + 128;
      //setPixel(i,level,0,0);
      //float level = sin(i+Position) * 127 + 128;
      pixels.setPixelColor(i,((sin(i + running_lights_state) * 127 + 128) / 255) * red,
                             ((sin(i + running_lights_state) * 127 + 128) / 255) * green,
                             ((sin(i + running_lights_state) * 127 + 128) / 255) * blue);
    }
    
    pixels.show();
    delay(WaveDelay);
}

// This function turns on the entire strip to color and randomly brightnes one pixel.
// If color is already at at maximum brightness this function will do nothing.
void snow_sparkle(uint32_t color, int brightenAmount, int sparkleSpawnRate, int sparkleTime) {
    set_color(color);
    if(random(sparkleSpawnRate) == 1){
        int i = random(NUMPIXELS);
        pixels.setPixelColor(i, pixels.Color(getPixelColorComponent(i, 1) + brightenAmount, getPixelColorComponent(i, 2) + brightenAmount, getPixelColorComponent(i, 3) + brightenAmount));
        pixels.show();
        delay(sparkleTime);
        pixels.setPixelColor(i, color);
        pixels.show(); 
    }
    delay(10);
}

// randomly turn some pixels on and off. 
// it is supposed to be evocative of sparklying sunlight on a surface (e.g. a lake)
void sparkle(uint32_t color, int SpeedDelay) {
    int i = random(NUMPIXELS);
    pixels.setPixelColor(i, color);
    pixels.show();
    delay(SpeedDelay);
    clear_leds();
}

// The most obnoxious lighting in existence.
// I recommend not including this for the sake of public health.
void strobe(uint32_t color, int strobeSpeed){
    set_color(color);
    pixels.show();
    delay(strobeSpeed);
    clear_leds();
    pixels.show();
    delay(strobeSpeed);
}

// Theatre-style crawling lights.
void theater_chase(uint32_t c, uint8_t wait) {
    theaterChaseState++;
    theaterChaseState %= 3;
    for (int i = 0; i < pixels.numPixels(); i = i + 3) {
      pixels.setPixelColor(i + theaterChaseState, c);    //turn every third pixel on
    }
    pixels.show();

    delay(wait);

    for (int i=0; i < pixels.numPixels(); i=i+3) {
        pixels.setPixelColor(i + theaterChaseState, 0);        //turn every third pixel off
    }
}

// moves a rainbow across the led strip. rainbow equally distributed throughout
void rainbow_cycle(uint8_t wait) {
    uint16_t i;

    rainbowCycleState++;
    rainbowCycleState %= 256;
    
    for(i=0; i< pixels.numPixels(); i++) {
        pixels.setPixelColor(i, wheel(((i * 256 / pixels.numPixels()) + rainbowCycleState) & 255));
    }
    
    pixels.show();
    delay(wait);
}

// return a color on the color wheel using wheel pos as the position
uint32_t wheel(byte wheelPos) {
  wheelPos = 255 - wheelPos;
  if(wheelPos < 85) {
    return pixels.Color(255 - wheelPos * 3, 0, wheelPos * 3);
  }
  if(wheelPos < 170) {
    wheelPos -= 85;
    return pixels.Color(0, wheelPos * 3, 255 - wheelPos * 3);
  }
  wheelPos -= 170;
  return pixels.Color(wheelPos * 3, 255 - wheelPos * 3, 0);
}

// read the unput from a mic and visualize the intensity as two mirrored bars on the led strip
void music_visualizer(){
   unsigned long startMillis= millis();  // Start of sample window
   unsigned int peakToPeak = 0;   // peak-to-peak level
 
   unsigned int signalMax = 0;
   unsigned int signalMin = 1024;
 
   // collect data for 50 mS
   while (millis() - startMillis < sampleWindow)
   {
      sample = analogRead(0);
      if (sample < 1024)  // toss out spurious readings
      {
         if (sample > signalMax)
         {
            signalMax = sample;  // save just the max levels
         }
         else if (sample < signalMin)
         {
            signalMin = sample;  // save just the min levels
         }
      }
   }
   peakToPeak = signalMax - signalMin;  // max - min = peak-peak amplitude
   double volts = (peakToPeak * 3.3) / 1024;  // convert to volts
   
   increment_rgb();
   meter_leds(volts);
}

// Light up from both ends up to the pixel specified by max_val
// max_val is expected in volts
void meter_leds(float max_val){
  for(int i=0; i < NUMPIXELS/2; i++){
      if(max_val >= led_scale(i, 0, NUMPIXELS/2) + peak_to_peak_dc){
         pixels.setPixelColor(i, rgbColor[0], rgbColor[1], rgbColor[2]);
         pixels.setPixelColor(NUMPIXELS - i - 1, rgbColor[0], rgbColor[1], rgbColor[2]);
      }
      else{
         pixels.setPixelColor(i, 0, 0, 0); // off
         pixels.setPixelColor(NUMPIXELS - i - 1, 0, 0, 0); // off
      }
  }
  pixels.show(); // This sends the updated pixel color to the hardware. 
}

// In: an integer in the range of the led strip (min_val to max_val) and the min and max values of that input
// Out: the inputs scaled to the range of possible voltages  
float led_scale(float val, float min_val, float max_val){
  return ((max_mic_volts - min_mic_volts) * (val - min_val) / (max_val - min_val)) + min_mic_volts;
}

// cycle through all colors
void increment_rgb(){
    rgbColor[decColor] -= 1;
    rgbColor[incColor] += 1;
    
    if(rgbColor[decColor] == 0){
      decColor += 1;
      decColor %= 3;
      incColor = decColor == 2 ? 0 : decColor + 1;
    }
}

// a long bright head with a long tail moves along the pixel strip
void pulse_wave(uint32_t headColor, uint8_t tailLength, double fadeRate, uint8_t wait){
    // move the pulse along
    for(int i = NUMPIXELS; i > 0; i--){
        pixels.setPixelColor(i, pixels.getPixelColor(i - 1));
    }


    // spawn more of the pulse tail
    int firstHeadPos = star_fall_find_head(headColor, 0, NUMPIXELS, 1); 
    if(firstHeadPos != -1 && firstHeadPos < tailLength + 1){
        if(firstHeadPos < tailLength){
            pixels.setPixelColor(0, pixels.Color( getPixelColorComponent(1, 1) * fadeRate,
                                                       getPixelColorComponent(1, 2) * fadeRate,
                                                       getPixelColorComponent(1, 3) * fadeRate));
        }else{
            pixels.setPixelColor(0, pixels.Color( 0, 0, 0 ));
        }

        //pixels.setPixelColor(center - 1, pixels.getPixelColor(center));                                           
    }

    // spawn new pulse if no pulse currently exists
    if(star_fall_find_head(headColor, 0, NUMPIXELS, 1) == -1){
        pixels.setPixelColor(0, headColor);
    }

    pixels.show();
    delay(wait);
}

// pulse waves are randomly spawned from the center of the strip and travel randomly to the left or the right
void star_fall(uint32_t color, uint8_t tailLength, double fadeRate, int spawnRate, uint8_t wait){
    if(NUMPIXELS % 2 == 1){
        maintain_star_fall_odd(color, tailLength, spawnRate, fadeRate);
    }else{
        maintain_star_fall_even(color, tailLength, spawnRate, fadeRate);
    }
    pixels.show();
    delay(wait);
}

// checks the strip for in progress star fall patterns and moves them forward
void maintain_star_fall_odd(uint32_t headColor, uint32_t tailLength, int spawnRate, double fadeRate){
    // choose direction for a new star to go
    int center = NUMPIXELS / 2;
    if(pixels.getPixelColor(center) == headColor){
        if(random(2) == 1){
           pixels.setPixelColor(center + 1, headColor);
        }else{
           pixels.setPixelColor(center - 1, headColor);
        }

        if(tailLength > 0){
            pixels.setPixelColor(center, pixels.Color( getPixelColorComponent(center, 1) * fadeRate,
                                                       getPixelColorComponent(center, 2) * fadeRate,
                                                       getPixelColorComponent(center, 3) * fadeRate));
        }
    }

    // maintain towards to the beginning of the led strip
    int endPos;
    endPos = center - 1;

    // move the beginning side pattern along
    for(int i = 0; i < endPos; i++){
        pixels.setPixelColor(i, pixels.getPixelColor(i + 1));
    }
  
    // maintain towards to the end of the led strip
    endPos = center + 1;

    // move the ending side pattern along
    for(int i = NUMPIXELS - 1; i > endPos; i--){
        pixels.setPixelColor(i, pixels.getPixelColor(i - 1));
    }

    //spawn more beginning side tail (if it exists)
    int firstHeadPos = star_fall_find_head(headColor, center - 1, -1, -1); 
    if(firstHeadPos != -1 && (center - firstHeadPos) < tailLength + 1){
        if((center - firstHeadPos) < tailLength){
            pixels.setPixelColor(center, pixels.Color( getPixelColorComponent(center, 1) * fadeRate,
                                                      getPixelColorComponent(center, 2) * fadeRate,
                                                       getPixelColorComponent(center, 3) * fadeRate));
        }else{
            pixels.setPixelColor(center, pixels.Color( 0, 0, 0 ));
        }

        pixels.setPixelColor(center - 1, pixels.getPixelColor(center));                                           
    }
    
    //spawn more ending side tail (if it exists)
    firstHeadPos = star_fall_find_head(headColor, center + 1, NUMPIXELS, 1); 
    if(firstHeadPos != -1 && (firstHeadPos - center) < tailLength + 1){
        if((firstHeadPos - center) < tailLength){
            pixels.setPixelColor(center, pixels.Color( getPixelColorComponent(center, 1) * fadeRate,
                                                      getPixelColorComponent(center, 2) * fadeRate,
                                                       getPixelColorComponent(center, 3) * fadeRate));
        }else{
            pixels.setPixelColor(center, pixels.Color( 0, 0, 0 ));
        }
          pixels.setPixelColor(center + 1, pixels.getPixelColor(center));   
    }

    // spawn a new star fall
    if (random(spawnRate) == 1) {
        pixels.setPixelColor(NUMPIXELS / 2, headColor);
    }
}

// moves the star fall pattern along the pixels
void maintain_star_fall_even(uint32_t headColor, uint32_t tailLength, int spawnRate, double fadeRate){
    // spawn a new star fall
    int beginCenter = NUMPIXELS / 2 - 1;
    int endCenter = NUMPIXELS / 2;

    // move the beginning side pattern along
    for(int i = 0; i < beginCenter; i++){
        pixels.setPixelColor(i, pixels.getPixelColor(i + 1));
    }

    // move the ending side pattern along
    for(int i = NUMPIXELS; i > endCenter; i--){
        pixels.setPixelColor(i, pixels.getPixelColor(i - 1));
    }

    //spawn more beginning side tail (if it exists)
    int firstHeadPos = star_fall_find_head(headColor, beginCenter, -1, -1); 
    if(firstHeadPos != -1 && (beginCenter - firstHeadPos) < tailLength + 1){
        if((beginCenter - firstHeadPos) < tailLength){
            pixels.setPixelColor(beginCenter, pixels.Color( getPixelColorComponent(beginCenter, 1) * fadeRate,
                                                            getPixelColorComponent(beginCenter, 2) * fadeRate,
                                                            getPixelColorComponent(beginCenter, 3) * fadeRate));
        }else{
            pixels.setPixelColor(beginCenter, pixels.Color( 0, 0, 0 ));
        }
    }
    
    //spawn more ending side tail (if it exists)
    firstHeadPos = star_fall_find_head(headColor, endCenter, NUMPIXELS, 1); 
    if(firstHeadPos != -1 && (firstHeadPos - endCenter) < tailLength + 1){
        if((firstHeadPos - endCenter) < tailLength){
            pixels.setPixelColor(endCenter, pixels.Color( getPixelColorComponent(endCenter, 1) * fadeRate,
                                                       getPixelColorComponent(endCenter, 2) * fadeRate,
                                                       getPixelColorComponent(endCenter, 3) * fadeRate));
        }else{
            pixels.setPixelColor(endCenter, pixels.Color( 0, 0, 0 ));
        }
    }

    // spawn a new star
    if (random(spawnRate) == 1) {
        uint16_t i = random(beginCenter, endCenter + 1);
        pixels.setPixelColor(i, headColor);
    }
}

// searches the pixels from searchStart towards searchEnd in searchDir direction for a pixel withh headColor.
// search dir must be 1 or -1
// iteratively applying searchDir to searchStart should eventually reach searchEnd
int star_fall_find_head(uint32_t headColor, int searchStart, int searchEnd, int searchDir){
    if(searchDir != 1 && searchDir != -1){
        return -1;
    }

    for(int i = searchStart; i != searchEnd; i += searchDir){
        if(pixels.getPixelColor(i) == headColor){
            return i;
        }
    }
    return -1;
}

// flickers the pixels in a way that is supposed to be evocative of fire
void  fire_pulse(){
    int r = 255;
    int g = r-40;
    int b = 40;

    for(int x = 8; x <99; x++){
        int flicker = random(0,150);
        int r1 = r-flicker;
        int g1 = g-flicker;
        int b1 = b-flicker;
        if(g1 < 0) g1=0;
        if(r1 < 0) r1=0;
        if(b1 < 0) b1=0;
        pixels.setPixelColor(x,r1,g1, b1);
    }
    pixels.show();
    delay(random(50,150));
}

// randomly sets a pixel to full brightness, then darkens it until it is off
void twinkle(uint32_t c, int twinkle_rate){
    // Random spawn
    if (random(twinkle_rate) == 1) {
        uint16_t i = random(NUMPIXELS);
        pixels.setPixelColor(i, c);
    }

    // fade out
    for(uint16_t i = 0; i < NUMPIXELS; i++){
       if(getPixelColorComponent(i, 1) > 1 || getPixelColorComponent(i, 2) > 1 || getPixelColorComponent(i, 3) > 1){
          pixels.setPixelColor(i, pixels.Color( getPixelColorComponent(i,1) * twinkleFadeRate,
                                                getPixelColorComponent(i,2) * twinkleFadeRate,
                                                getPixelColorComponent(i,3) * twinkleFadeRate));
        }else{
          pixels.setPixelColor(i, pixels.Color(0, 0, 0));
        }
    }
    pixels.show();
    delay(10);
}

// makes the entire strip dim and brighten
void breathe(uint32_t c, float maxBrightness, float breatheSpeed, int breathDelay){
    if(breatheState == 0){
        clear_leds();
    }
    int breathePeriod = 2 * PI / breatheSpeed;
    
    breatheState++;
    if(breatheState == breathePeriod){
        breatheState = 0;
    }
      
      
    float intensity = (maxBrightness / 2.0) * (1.0 + sin(breatheSpeed * breatheState - PI/2));
    set_color(c);
    setBrightness(intensity);
    pixels.show();
    delay(breathDelay);
}

// brighten, then darken the color.
// switches colors randomly when brightness == 0
void colored_breathe(float maxBrightness, float breatheSpeed, int breathDelay){
    if(breatheState == 0){
        breatheColor = pixels.Color(random(255), random(255), random(255));
    }
    int breathePeriod = 2 * PI / breatheSpeed;
    
    breatheState++;
    if(breatheState == breathePeriod){
        breatheState = 0;
    }
      
      
    float intensity = (maxBrightness / 2.0) * (1.0 + sin(breatheSpeed * breatheState - PI/2));
    set_color(breatheColor);
    setBrightness(intensity);
    pixels.show();
    delay(breathDelay);
}

/************************
 *                      *
 *  Utility Functions   *
 *                      *
 ************************/


// the color component segment of specified pixel
// 1 = red, 2 = blue, 3 = green
uint8_t getPixelColorComponent(uint8_t pixelNum, uint8_t segment){
  uint8_t segmentColorValue;
  uint32_t c = pixels.getPixelColor(pixelNum);
  switch( segment ){
    case 1:
      segmentColorValue = ( c >> 16) & 255;
      break;
    case 2:
      segmentColorValue = ( c >> 8) & 255;
      break;
    case 3:
      segmentColorValue = c & 255;
      break;
  }
  return segmentColorValue;
}

// the color component segment of the color c
// 1 = red, 2 = blue, 3 = green
uint8_t getColorComponent(uint32_t c, uint8_t segment){
  uint8_t segmentColorValue;
  switch( segment ){
    case 1:
      segmentColorValue = ( c >> 16) & 255;
      break;
    case 2:
      segmentColorValue = ( c >> 8) & 255;
      break;
    case 3:
      segmentColorValue = c & 255;
      break;
  }
  return segmentColorValue;
}

// set all leds to brightness
void setBrightness(float brightness){
    for(int i = 0; i < NUMPIXELS; i++){
        pixels.setPixelColor(i, pixels.Color( brightness * getPixelColorComponent(i, 1) / ((float)255),
                                              brightness * getPixelColorComponent(i, 2) / ((float)255),
                                              brightness * getPixelColorComponent(i, 3) / ((float)255)));
    }
}

// set all leds to color c
void set_color(uint32_t c){
    for(int i = 0; i < NUMPIXELS; i++){
       pixels.setPixelColor(i, c);
    }
}

// turns off all leds
void clear_leds(){
   for(int i=0; i<NUMPIXELS; i++){
      pixels.setPixelColor(i, pixels.Color(0,0,0));
  }
  pixels.show();
}
