#include <Arduino.h>
#include <Servo.h>
#include <HCSR04.h>
#include <PID_2.h>

#define TRIG_PIN 12
#define ECHO_PIN 11
float measurement;
float filtered_measurement = 20;

#define servoPin 9
int minServoValue = 480;
int maxServoValue = 2500;
Servo servo;

void set_servo();
void home();
void check_home();

int minServoAngle = 65;
int maxServoAngle = 115;
int HomeServoAngle = 90;

float maxLinearDistance = 60.0;
float minLinearDistance = 0.0;
// float Setpoint = (maxLinearDistance - minLinearDistance)/2 + minLinearDistance;
float Setpoint = 20;
float average_measurement = 20;
float error, momentum;

int button_pin = 6;

UltraSonicDistanceSensor sensor(TRIG_PIN, ECHO_PIN);

// const float k_P = -50, k_I = -1.25, k_D = -0.25, T = 0.005;
const float k_P = -2.50, k_I = -0.50, k_D = -0.0150, T = 0.005;

PID pid(k_P, k_I, k_D, T);

void setup()
{
  Serial.begin(9600);
  servo.attach(servoPin, minServoValue, maxServoValue);
  pinMode(button_pin, INPUT_PULLUP);
  servo.write(HomeServoAngle);
  pid.setNormalize(maxLinearDistance, minLinearDistance, maxServoAngle, minServoAngle);

  delay(500);
}

bool run = false;
long sum;
void loop()
{
  sum = 0;
  for (int i = 0; i < average_measurement; i++)
  {
    measurement = sensor.measureDistanceCm();
    filtered_measurement = 0.75 * filtered_measurement + 0.25 * measurement;
    measurement = filtered_measurement;
    sum += measurement;
  }
  measurement = sum / average_measurement;
  // Low Pass Filter
  // filtered_measurement = 0.75 * filtered_measurement + 0.25 * measurement;
  // measurement = filtered_measurement;

  if (measurement >= minLinearDistance && measurement <= maxLinearDistance)
  {
    run = true;
    error = Setpoint - measurement;
    float angle_pos = pid.computePID(error);

    String vars = pid.getVariables();

    Serial.print("*/" + String(measurement) + "," + String(error) + "," + String(angle_pos) + vars + "/*");
    Serial.println();
    if (minServoAngle <= angle_pos < maxServoAngle){
    servo.write(round(angle_pos));}
    check_home();
  }

  check_home();

  // if(measurement <= minLinearDistance){
  //   servo.write(servo.read()-10);
  //   delay(100);
  // }
  // else if(measurement >= maxLinearDistance){
  //   servo.write(servo.read()+10);
  //   delay(100);
  // }
  if (run == false){
  Serial.print("*/" + String(measurement) + "," + String(0) + "," + String(0) + "/*");}
  Serial.println();
  run = false;
}

void check_home()
{
  if (digitalRead(button_pin) == LOW)
  {
    home();
  }
}

void home()
{
  servo.write(HomeServoAngle);
  delay(250);
}