#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN 6

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(30, PIN, NEO_GRB + NEO_KHZ800);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

void setup() {
  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
  #if defined (__AVR_ATtiny85__)
    if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
  #endif
  // End of trinket special code

  Serial.begin(9600);
  strip.begin();
  strip.setBrightness(125);
  strip.show(); // Initialize all pixels to 'off'
}


int delayTime = 0;
int rawAnalog = 0;
int mode = 0;

//Shades of Blue
uint32_t midnight_blue = strip.Color(25, 25, 112);
uint32_t navy_blue = strip.Color(0, 0, 128);
uint32_t dark_blue = strip.Color(0, 0, 139);
uint32_t ucsb = strip.Color(0, 70, 139);
uint32_t pigment_blue = strip.Color(51, 51, 153);
uint32_t egyptian_blue = strip.Color(16, 52, 166);
uint32_t cerulean = strip.Color(0, 123, 167);
uint32_t liberty = strip.Color(84, 90, 167);
uint32_t cobalt = strip.Color(0, 71, 171);
uint32_t munsell_blue = strip.Color(0, 147, 175);
uint32_t steel_blue = strip.Color(70, 130, 180);
uint32_t baby_blue = strip.Color(137, 207, 240);
uint32_t cerulean2 = strip.Color(0, 63, 255);
uint32_t blue = strip.Color(0,0,255);
uint32_t azure = strip.Color(0, 127, 255);
uint32_t cyan = strip.Color(0, 255, 255);

uint32_t blueShades[] = {midnight_blue, navy_blue, dark_blue, ucsb, pigment_blue, egyptian_blue, cerulean,
                         liberty, cobalt, munsell_blue, steel_blue, baby_blue, cerulean2, blue, azure, cyan};

uint32_t red = strip.Color(255,0,0);
uint32_t orange = strip.Color(255,55,0);
uint32_t yellow = strip.Color(255,255,0);
uint32_t green = strip.Color(0,255,0);

uint32_t purple = strip.Color(255,0,255);

uint32_t rainbowArray[] = {red, orange, yellow, green, blue, purple};
String rainbowLabels[] = {"red", "orange", "yellow", "green", "blue", "PURPLE"};


#define ARRAYSIZE(x) (sizeof(x)/sizeof(*x))

void loop() {
  //Receive inputs from serial connection and
  // change mode accordingly, otherwise do default mode
  
  if (Serial.available()>0){
     mode = Serial.parseInt();
     
     if (Serial.read() == '#'){
     //Check for code marker
     
       if (mode == 1){
          tetris();  
       }
     } 
  }
  
  //Default behavior
  //colorfulPersonalityPart1();

  //colorfulPersonalityIncreasing(20);
  //colorfulPersonalityPart2(1, 30, 50, 0);
  //colorfulPersonalityPart1();

  //blueRainbow(100);

  //blueMoon(1,strip.numPixels(),100);
  //stepUp(navy_blue,10);

  //randomBlue(15);
  //randomBlueAlt(5);
  
  //blueTetris(10, 300);
  blueTetrisAlt();
  
  //bluePersonality(1, 30, 20, 20);
  //bluePersonalityAlt(1, 30, 20);

  /*int whatever[] = {12, 53, 44};
  Serial.println(whatever[-1]);*/
}

void blueMoon(int width, int endLight, int delayTime) {
  //Cycle through all the lights,
  // Change color based on color wheel
  for(int j=110; j<155; j++) {
    for(int i=0; i<endLight; i++) {
      //Turn i-1 light off
      strip.setPixelColor(arrayMap(i-1,endLight), strip.Color(0,0,0));
      strip.show();

      //Turn i on
      strip.setPixelColor(arrayMap(i,endLight), Wheel((i+j) & 255));
      strip.show();
      delay(delayTime);
    }
  }
}

void bluePersonality(int width, int endLight, int delayTime) {
  //Cycles through lights using alternate algorithm, better for leaving behind
  // lights for use in the tetris mode, due to the initial condition of
  // setting previousLight as 0;
  int previousLight = 0;
  int currentLight = 0;

  //Cycle through all the lights
  for(uint16_t i=0; i<endLight; i++) {
    // If done cycling through list, reset

    currentLight = i;
    previousLight = currentLight-width;

    if (previousLight<0){
      previousLight+=endLight;
    }

    //Turn previous light off
    strip.setPixelColor(previousLight, strip.Color(0,0,0));
    strip.show();
    delay(delayTime);

    //Turn current light on
    strip.setPixelColor(i, Wheel((i+110) & 255));
    strip.show();
    delay(delayTime);
  }
}

void blueTetris(int delayTime, int delayTime2){
  //Animation 1: Building up
  for (int i = strip.numPixels(); i>0; i--){
    //the ending light decreases, which leaves the previous endPoint remaining lit
     bluePersonality(1,i,delayTime);
  }

  //Animation 2: Flash and Disappear
  delay(delayTime2);
  allOn(0, delayTime2);
  allOn(steel_blue, delayTime2);
  allOn(0, delayTime2);
  allOn(steel_blue, delayTime2);
}

void blueTetrisAlt(){
  //when light reaches the end, it stays lit
  int width = 1;
  int delayTime = 5;
  for (int k = strip.numPixels(); k>0; k--){
    int previousLight = 0;
    int currentLight = 0;
    int endLight = k;
  
    //Cycle through all the lights
    for(uint16_t i=0; i<endLight; i++) {
      // If done cycling through list, reset
      currentLight = i;
      previousLight = currentLight-width;
  
      if (previousLight<0){
        previousLight+=endLight;
      }
  
      //Turn previous light off
      strip.setPixelColor(previousLight, strip.Color(0,0,0));
      strip.show();
      delay(delayTime);
  
      //Turn current light on
      strip.setPixelColor(i, Wheel((i+110) & 255));
      strip.show();
      delay(delayTime);
    }

    delay(100);
    //Change color of the static lights
    for (int m = strip.numPixels(); m >= endLight-2; m--){
      strip.setPixelColor(m, Wheel((k+m+endLight+255) & 255));
      Serial.print(endLight);
      Serial.print("---");
      Serial.println(m);
    }
    delay(100);
  }
 delay(2000);

 for (int m = strip.numPixels(); m >=0; m--){
  strip.setPixelColor(m, Wheel((m+0+255) & 255));
}
  
}


int arrayMap(int n){
  //Simplifies the modulo operations
  return (n+strip.numPixels()) % strip.numPixels();
}

int arrayMap(int n, int a){
  //Simplifies the modulo operations
  return (n+a) % a;
}

//110-185 COLOR RANGE
void blueRainbow(uint8_t wait) {
  //Adapted from rainbow(), limiting it to the blue portion of the color wheel
  //Goes back and forth for fluid motion
  uint16_t i, j;

  for(j=110; j<155; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
  for(j=155; j>110; j--) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

void stepUp(uint32_t c, int wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    for (int k=3; k>0; k--){
      strip.setPixelColor(arrayMap(i+k), c);
      strip.show();
      delay(wait);
      
      strip.setPixelColor(arrayMap(i+k), strip.Color(0,0,0));
      strip.show();
    }
  }
}

void randomBlue(int wait) {
  //Turns on a random light using a random blue tint from the blueshades array
  //Turns off a random light
  strip.setPixelColor(random(0,strip.numPixels()), blueShades[random(0,15)]);
  strip.show();
  delay(wait);

  strip.setPixelColor(random(0,strip.numPixels()), strip.Color(0,0,0));
  strip.show();
  delay(wait);
}

void randomBlueAlt(int wait) {
  //Slightly different from randomBlue, turns on a random number of lights from 1 to 5
  // then turns off a random number of lights from 1 to 5
  for (int i=0; i<random(1,5);i++){
    strip.setPixelColor(random(0,strip.numPixels()), blueShades[random(0,15)]);
    strip.show();
    delay(wait);
  }
  for (int i=0; i<random(1,5);i++){
    strip.setPixelColor(random(0,strip.numPixels()), strip.Color(0,0,0));
    strip.show();
    delay(wait);
  }
}

void allOn(uint32_t c, int wait) {
  //Solid color for all lights
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
  }
  strip.show();
  delay(wait);
}

void bluePersonalityAlt(int width, int endLight, int delayTimeOn) {
  //Mode suggestion:
    // should make optional constructors
  int previousLight = 0;
  int currentLight = 0;

  //Cycle through all the lights
  for(int i=0; i<endLight; i++) {
    // If done cycling through list, reset

    currentLight = i;
    previousLight = currentLight-width;

    if (previousLight<0){
      previousLight+=endLight;
    }

    //Turn current light on
    strip.setPixelColor(currentLight, Wheel((i+110) & 255));
    strip.show();
    delay(delayTimeOn);

    //Turn previous light off
    strip.setPixelColor(previousLight, strip.Color(0,0,0));
    strip.show();
    delay(delayTimeOn);
  }
}

void tetris(){
  //when light reaches the end, it stays lit
  for (int i = strip.numPixels(); i>0; i--){
     colorfulPersonalityPart2(1,i,20,0);
  }
}

void colorfulPersonalityPart2(int width, int endLight, int delayTimeOn, int delayTimeOff) {
  //Mode suggestion:
    // should make optional constructors
  int x =0;
  int previousLight = 0;
  int currentLight = 0;

  //Cycle through all the lights
  for(uint16_t i=0; i<endLight; i++) {
    // If done cycling through list, reset
    if (x>5){
      x=0;
    }

    currentLight = i;
    previousLight = currentLight-width;

    if (previousLight<0){
      previousLight+=endLight;
    }

    //Turn previous light off
    strip.setPixelColor(previousLight, strip.Color(0,0,0));
    strip.show();
    delay(delayTimeOff);
    
    strip.setPixelColor(currentLight, rainbowArray[x]);
    
    x++;
    strip.show();
    delay(delayTimeOn);
  }
}

void colorfulPersonalityIncreasing(int delayTime){
  for (int i =0; i<strip.numPixels();i++){
  colorfulPersonalityPart2(i,strip.numPixels(),delayTime,0);
  }
}

void colorfulPersonalityPart1() {
  int x =0;
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    //Cycle through all the lights (0-30)
    //Cycle through all the colors and reset at 6
    if (x>5){
      x=0;
    }

    //Set the light to a color
    strip.setPixelColor(i, rainbowArray[x]);

    x++;
    strip.show();
  }
  //delay(1000000);
}

void colorWipe_Forward(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    Serial.println(i);
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

void colorWipe_Backward(uint32_t c, uint8_t wait) {
  for(uint16_t i=strip.numPixels(); i>0; i--) {
    Serial.println(i);
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
      Serial.print((i+j));
      Serial.print("----");
      Serial.println((i+j) & 255);
    }
    strip.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
