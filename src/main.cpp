#include <Arduino.h>
#include "HCSR04.h"

#define TRIG_PIN 12
#define ECHO_PIN 11
float a;

// Define the pin numbers
const int pinr = 6;
const int ping = 5;
const int pinb = 3;
uint32_t Wheel(byte );

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
      int color = Wheel(map(a, 4, 50, 0, 255)); // Map distance to a rainbow color
      analogWrite(pinr, (color >> 16) & 0xFF); // Red component
      analogWrite(ping, (color >> 8) & 0xFF);  // Green component
      analogWrite(pinb, color & 0xFF);         // Blue component
    }
  }

  delay(100);
}

// Generate rainbow color based on a value from 0 to 255
uint32_t Wheel(byte WheelPos)
{
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85)
  {
    return 0xFF - WheelPos * 3;
  }
  if (WheelPos < 170)
  {
    WheelPos -= 85;
    return 0xFF - WheelPos * 3 - 0x0000FF00;
  }
  WheelPos -= 170;
  return 0xFF0000 - WheelPos * 3;
}