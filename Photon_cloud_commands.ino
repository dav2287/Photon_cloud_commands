/*
 This can be used to send commands to the Particle Photon using the particle-CLI

 particle call 0123456789ABCDEFGHI command {args}
    where {args} is dependent on 'command'


 ****************
 WORK IN PROGRESS
 ****************

 */

#include "application.h"
#include "neopixel.h"

SYSTEM_MODE(AUTOMATIC);

// IMPORTANT: Set pixel COUNT, PIN and TYPE
#define PIXEL_PIN D2
#define PIXEL_COUNT 10
#define PIXEL_TYPE WS2812B

int rgb[3];
int pin;
String command;

int setAll();
int setColor();
int setPixel();
int setBrightness();

Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, PIXEL_TYPE);

void setup()
{
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  strip.setBrightness(50);
  Particle.function("parseData", parseData);
  Serial.begin(9600);
}

void loop()
{
  // setAll(0x000000);
  // rainbow(20);
}

int setAll() {

    // parseData(inData);

    uint16_t i;

    for(i=0; i<strip.numPixels(); i++) {
        strip.setPixelColor(i, rgb[0], rgb[1], rgb[2]);
        strip.show();
    }
    return 1;
}

int parseData(String inData) {
    char inputStr[64];
    inData.toCharArray(inputStr,64);
    char *p = strtok(inputStr,",");
    command = p;
    p = strtok(NULL,",");

    int values = numValues(inData);
    Serial.println(values);


    if (values == 4) {
        pin = atoi(p);
        p = strtok(NULL,",");
        values -= 1;
        Serial.println(pin);
    }
    if (values == 3) {
        rgb[0] = atoi(p);
        p = strtok(NULL,",");
        rgb[1] = atoi(p);
        p = strtok(NULL,",");
        rgb[2] = atoi(p);
        p = strtok(NULL,",");
        Serial.println(rgb[0]);
        Serial.println(rgb[1]);
        Serial.println(rgb[2]);
    }
    Serial.println(command);
    Serial.println(command == "setPin");
    if (command == "setPin") {
        setPin();
    }
    if (command == "setAll") {
        setAll();
    }
    return 1;
}

int numValues(String inData) {
    int values = 0;
    int from = inData.indexOf(",");
    while (true) {
        if (from == -1) {
            break;
        }
        values += 1;
        from = inData.indexOf(",",from+1);
    }
    Serial.println(values);
    return values;
}

int setPin() {
    // parseData(inData);
    strip.setPixelColor(pin, rgb[0], rgb[1], rgb[2]);
    strip.show();
    return 1;
}

int setBright(String inData) {
    int brightness = inData.toInt();
    strip.setBrightness(brightness);
    strip.show();
    return 1;
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

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
   return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}
