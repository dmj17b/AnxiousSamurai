#include <Arduino.h>
#include <FastLED.h>
#ifdef ESP8266
#include <ESP8266WiFi.h>
#else
#include <WiFi.h>
#endif
#include <WiFiUdp.h>
#include <OSCMessage.h>
#include <OSCBundle.h>
#include <OSCData.h>
#include <time.h>

// Setting up the WiFi connection
char ssid[] = "911Wifi";          // your network SSID (name)
char pass[] = "Seminoles1!";     // your network password

// A UDP instance to let us send and receive packets over UDP
WiFiUDP Udp;
const IPAddress outIp(10,40,10,105);        // remote IP (not needed for receive)
const unsigned int outPort = 9999;          // remote port (not needed for receive)
const unsigned int localPort = 8888;        // local port to listen for UDP packets (here's where we send the packets)
OSCErrorCode error;


// Number of LEDs in the strip
#define NUM_LEDS 60

// For led chips like WS2812, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806 define both DATA_PIN and CLOCK_PIN
// Clock pin only needed for SPI based chipsets when not using hardware SPI
#define DATA_PIN 25

// Define the array of leds
CRGB leds[NUM_LEDS];

int lightState = 0;

// Different LED effects
void loadingBar(CRGB color,int speed);
void splotchyFun(CRGB color, int speed);
void flameLEDs();
void heaven();
void ocean();
void glamorous();



/**************** SETUP *****************/
void setup() { 
    FastLED.addLeds<WS2812B, DATA_PIN, RGB>(leds, NUM_LEDS);  // GRB ordering is typical
    Serial.begin(115200);
    // Connect to WiFi network
    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, pass);

    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println("");

    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    Serial.println("Starting UDP");
    Udp.begin(localPort);
    Serial.print("Local port: ");
  #ifdef ESP32
    Serial.println(localPort);
  #else
    Serial.println(Udp.localPort());
  #endif

}


/**************** LOOP *****************/
void loop() { 
  OSCMessage msg;
  int size = Udp.parsePacket();
    if (size > 0) {
    while (size--) {
      msg.fill(Udp.read());
    }
    if (!msg.hasError()) {
      Serial.print("Received message: ");
      lightState = msg.getInt(0);
      Serial.println(lightState);
    } else {
      error = msg.getError();
      Serial.print("error: ");
      Serial.println(error);
    }
  }

  // Switch statement to determine which LED effect to run
switch(lightState){
  case 0:
  // Turn off all LEDs
    for(int i = 0; i < NUM_LEDS; i++) {
      leds[i] = CRGB::Black;
    }
    FastLED.show();
    break;
  case 1:
  //Flame LED effect
    Serial.println("Flame effect");
    glamorous();
    break;
  case 2:
    Serial.println("Heaven effect");
    heaven();
    break;
  case 3:
    Serial.println("Flame effect");
    flameLEDs();
    break;  
  case 4:
    Serial.println("Ocean effect");
    ocean();  
    break;
  default:
  // Turn off all LEDs
    for(int i = 0; i < NUM_LEDS; i++) {
      leds[i] = CRGB::Black;
    }
    FastLED.show();
}

}




/*FUNCTIONS*/

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

// Function to create a splotchy effect with LED strip
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

// Orange and red flame vibes
void flameLEDs(){
  int i = random(0,NUM_LEDS);
  int r = 150+random(100);  // Random number between 150 and 255 (more red than green)
  int g = random(100);  // Random number between 0 and 100 (more green than blue)
  int b = 0;  //no blue
  leds[i] = CRGB(g, r, b);  // Set the LED to the random color
  FastLED.show();
  FastLED.setBrightness(255);
  delay(random(10));
}

// Pink and white heaven vibes
void heaven(){
  int i = random(0,NUM_LEDS);
  int r = 50+random(100);  // Random number between 150 and 255 (more red than green)
  int g = random(100);  // Random number between 0 and 100 (more green than blue)
  int b = 150+random(100);  //no blue
  leds[i] = CRGB(g, r, b);  // Set the LED to the random color
  FastLED.show();
  FastLED.setBrightness(255);
  delay(random(10));
}

// Blue and green ocean vibes
void ocean(){
  int t = micros()/1000000;
  Serial.println(t);
  int i = random(0,NUM_LEDS);
  int r = 0;  // Random number between 150 and 255 (more red than green)
  int g = 50+random(150);  // Random number between 0 and 100 (more green than blue)
  int b = 100+random(150);  //no blue
  leds[i] = CRGB(g, r, b);  // Set the LED to the random color
  FastLED.show();
  FastLED.setBrightness(255);
  delay(random(50));
}

// Glamorous effect
void glamorous(){
  for(int i = 0; i < NUM_LEDS; i++){
    float t = millis()/1000.5;
    int r = 5;  // Random number between 150 and 255 (more red than green)
    int g = 5;  // Random number between 0 and 100 (more green than blue)
    int b = 5;//*abs(cos(t));  //no blue
    leds[i] = CRGB(g,r,b);
  }
      FastLED.show();

  
 
}