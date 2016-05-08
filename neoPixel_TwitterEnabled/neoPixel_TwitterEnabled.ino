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
  strip.show(); // Initialize all pixels to 'off'
}


int delayTime = 0;
int rawAnalog = 0;
int mode = 0;
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
  colorfulPersonalityPart1();

}

//

uint32_t red = strip.Color(255,0,0);
uint32_t orange = strip.Color(255,55,0);
uint32_t yellow = strip.Color(255,255,0);
uint32_t green = strip.Color(0,255,0);
uint32_t blue = strip.Color(0,0,255);
uint32_t purple = strip.Color(255,0,255);

uint32_t rainbowArray[] = {red, orange, yellow, green, blue, purple};
String rainbowLabels[] = {"red", "orange", "yellow", "green", "blue", "PURPLE"};

//int myPins[] = {2, 4, 8, 3, 6};

void tetris(){
  //when light reaches the end, it stays lit

  for (int i = strip.numPixels(); i>0; i--){
     colorfulPersonalityPart2(1,i,20,0);
  }
}

void colorfulPersonalityIncreasing(){

  for (int i =0; i<strip.numPixels();i++){
  colorfulPersonalityPart2(i,strip.numPixels(),100,0);
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

void colorfulPersonalityPart3(int lights) {
  //Mode suggestion:
    // Building up
  int x =0;

  //Cycle through all the lights
  for(uint16_t i=0; i<strip.numPixels(); i++) {

    Serial.println("----");
    
    // If done cycling through list, reset
    if (x>5){
      x=0;
    }

    //if 
    int prev_i = i-3;
    if (prev_i<0){
      prev_i=strip.numPixels()-3;
    }

    
    
    strip.setPixelColor(i-3, strip.Color(0,0,0));

    
    strip.setPixelColor(i, rainbowArray[x]);
    Serial.print(i);
    Serial.print(" - on - ");
    Serial.println(rainbowLabels[x]);
    

    x++;
    strip.show();
    delay(100);
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
