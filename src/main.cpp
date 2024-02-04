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
#define DATA_PIN 12

// Define the array of leds
CRGB leds[NUM_LEDS];

int lightState = 0;

// Function prototypes
void loadingBar(CRGB color,int speed);
void splotchyFun(CRGB color, int speed);



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
      lightState = msg.getBoolean(0);
      Serial.println(msg.getBoolean(0));
    } else {
      error = msg.getError();
      Serial.print("error: ");
      Serial.println(error);
    }
  }

  if(lightState == 1){
    Serial.println("Light is on");
    for(int i=0; i<NUM_LEDS; i++) {
      leds[i] = CRGB(0,0,150);
      FastLED.show();
      FastLED.setBrightness(50);
      delay(50);
    }
  } else if(lightState == 0){
    for(int i=NUM_LEDS; i>=0; i--) {
      leds[i] = CRGB::Black;
      FastLED.show();
      delay(50);
    }
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
