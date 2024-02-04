#include <Arduino.h>
#include <FastLED.h>

// How many leds in your strip?
#define NUM_LEDS 60

// For led chips like WS2812, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806 define both DATA_PIN and CLOCK_PIN
// Clock pin only needed for SPI based chipsets when not using hardware SPI
#define DATA_PIN 12
#define CLOCK_PIN 13

// Define the array of leds
CRGB leds[NUM_LEDS];
void loadingBar(CRGB color,int speed);
void splotchyFun(CRGB color, int speed);

void setup() { 
    FastLED.addLeds<WS2812B, DATA_PIN, RGB>(leds, NUM_LEDS);  // GRB ordering is typical
    Serial.begin(115200);
}

void loop() { 


  loadingBar(CRGB(0,0,150), 50);
  // splotchyFun(CRGB(0,0,150), 50);

}



// Function to create a loading bar effect with LED strip
void loadingBar(CRGB color,int speed) {
  // Loop through all 60 LEDS
  for(int i=0; i<NUM_LEDS; i++) {
    leds[i] = CRGB(color);
    FastLED.show();
    FastLED.setBrightness(50);
    delay(speed);
  }
  // Loop back, turning them off:
  for(int i=NUM_LEDS; i>=0; i--) {
    leds[i] = CRGB::Black;
    FastLED.show();
    delay(speed);
  }

}

void splotchyFun(CRGB color, int speed){

  int groupIndex = 10;
  float sigma = 0.3;


 for(int i = 0; i < NUM_LEDS; i++) {
    // Randomly adjust the brightness of each LED

    float brightness = 1/(sigma*sqrt(2*PI)) * exp(-(1/2)*pow(((i-groupIndex)/sigma),2));
    Serial.println(brightness); 
    int adjust = random(10); // Random number between -32 and 32
    int r = leds[i].r + adjust;
    int g = leds[i].g + adjust;
    int b = leds[i].b + adjust;

    // Ensure RGB values are within 0-255
    r = constrain(r, 0, 255);
    g = constrain(g, 0, 255);
    b = constrain(b, 0, 255);

    leds[i] = CRGB(r, g, b);
  }

  FastLED.show();
  delay(100); // Delay for effect

}
