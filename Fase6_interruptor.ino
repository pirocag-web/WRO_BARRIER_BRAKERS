
#include <FastLED.h>
 #define NUM_LEDS 1
 #define DATA_PIN 4
 #define CLOCK_PIN 13
 CRGB leds[NUM_LEDS];
 int pulsadorPin = 2;
 int valorPulsador = 0;



void setup() {
  FastLED.addLeds<NEOPIXEL, 
DATA_PIN>(leds, NUM_LEDS);
 
 Serial.begin(9600);
 pinMode(pulsadorPin, INPUT);
}

void loop() {
// Leemos el valor del pin
 valorPulsador = digitalRead(pulsadorPin);
 if (valorPulsador == HIGH) {
 Serial.println(" esperando accion");
 leds[0] = CRGB::DeepSkyBlue;
 FastLED.show();
 }
 else {
 Serial.println(" pulsado");
 leds[0] = CRGB::Orange;
 FastLED.show();
 }
}
