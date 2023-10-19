#include <Arduino.h>

// Define the pin numbers
const int pinr = 6;
const int ping = 5;
const int pinb = 3;

// Define the colors
const int colors[4][3] = {{255, 165, 0}, {255, 255, 0}, {75, 0, 130}, {255, 0, 255}};

void setup() {
  // Set the pin modes
  pinMode(pinr, OUTPUT);
  pinMode(ping, OUTPUT);
  pinMode(pinb, OUTPUT);
}

void loop() {
  // Loop through each color
  for (int i = 0; i < 4; i++) {
    // Write each color to the LED
    analogWrite(pinr, colors[i][0]);
    analogWrite(ping, colors[i][1]);
    analogWrite(pinb, colors[i][2]);
    
    // Pause for a second before changing to the next color
    delay(500);
  }
}