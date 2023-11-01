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

void setup()
{
  Serial.begin(9600);
  myservo.attach(servoPin, min, max);
  pinMode(button_pin, INPUT_PULLUP);

  // set_servo();
  // delay(500);
  myservo.write(half_angle);
  delay(500);
}

void loop()
{
  a = sensor.measureDistanceCm();
  // int current_ang = myservo.read();

  if (a >= min_dist && a <= max_dist)
  {
    Serial.print("D: ");
    Serial.println(a);
    error = a - goal_dist;
    int momentum = PID.computePID(error);
    Serial.print("M: ");
    Serial.println(momentum);
    int angle = myservo.read() + momentum;
    if (angle >= min_angle and angle <= max_angle)
    {
      myservo.write(angle);
    }
  }

  if (digitalRead(button_pin) == LOW)
  {
    jiggle_servo(10);
  }
}

void set_servo()
{
  for (int angle = min_angle; angle <= max_angle; angle += 1)
  {
    myservo.write(angle);
    delay(1);
  }
  delay(100);
  for (int angle = max_angle; angle >= min_angle; angle -= 1)
  {
    myservo.write(angle);
    delay(1);
  }
}

void jiggle_servo(int move)
{
  int pos = myservo.read();
  myservo.write(move);
  delay(100);
  myservo.write(pos);
  delay(100);
  myservo.write(pos - move);
  delay(100);
  myservo.write(pos);
}