#include <Arduino.h>
#include <Servo.h>
#include <HCSR04.h>
#include <PID.h>

#define TRIG_PIN 12
#define ECHO_PIN 11
float a;

#define servoPin 9
int min = 480;
int max = 2500;
Servo myservo;

void set_servo();
void jiggle_servo(int move);

int min_angle = 15;
int max_angle = 170;
int half_angle = 90;

float max_dist = 42;
float min_dist = 5;
float goal_dist = 16.25;
float error;

int button_pin = 6;

int angle = 0;   // variable to store the servo position in degrees
int reading = 0; // variable to store the reading from the analog input

UltraSonicDistanceSensor sensor(TRIG_PIN, ECHO_PIN);

const float k_P = 0.5;
const float k_I = 0.05;
const float k_D = 0.001;
const float u_max = 10.0;
const float T = 0.1;

PIDController PID(k_P, k_I, k_D, u_max, T);

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