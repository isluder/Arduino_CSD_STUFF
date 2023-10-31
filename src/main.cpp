#include <Arduino.h>
#include "HCSR04.h"

#define TRIG_PIN 12
#define ECHO_PIN 11
float a;

// Define the pin numbers
const int pinr = 6;
const int ping = 5;
const int pinb = 3;

// Define the colors
const int colors[4][3] = {{255, 165, 0}, {255, 255, 0}, {75, 0, 130}, {255, 0, 255}};
void color_loop();

UltraSonicDistanceSensor sensor(TRIG_PIN, ECHO_PIN);

void setup()
{
  Serial.begin(9600);
  // Set the pin modes
  pinMode(pinr, OUTPUT);
  pinMode(ping, OUTPUT);
  pinMode(pinb, OUTPUT);
  delay(1000);
}

void loop()
{
  a = sensor.measureDistanceCm();
  if (a > 0)
  {
    Serial.print("*/");
    Serial.print(a);
    Serial.println("/*");
    if (a >= 4 && a <= 50)
    {
      color_loop();
      Serial.print("Color?");
    }
  }

  delay(100);
}

void color_loop()
{
  for (int i = 0; i < 4; i++)
  {
    // Write each color to the LED
    analogWrite(pinr, colors[i][0]);
    analogWrite(ping, colors[i][1]);
    analogWrite(pinb, colors[i][2]);

    // Pause for a second before changing to the next color
    delay(50);
  }
}