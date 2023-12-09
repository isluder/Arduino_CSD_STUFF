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

int minServoAngle = 75;
int maxServoAngle = 125;
int HomeServoAngle = 90;

float maxLinearDistance = 58.5;
float minLinearDistance = 4.7;
float Setpoint = 31.6;
float average_measurement = 5;
float error, momentum;

int button_pin = 6;

UltraSonicDistanceSensor sensor(TRIG_PIN, ECHO_PIN);

const float k_P = -40, k_I = -5, k_D = -1, T = 0.010;

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

long sum;
void loop()
{
  sum = 0;
  for (int i = 0; i < average_measurement; i++)
  {
    measurement = sensor.measureDistanceCm();
    sum += measurement;
  }
  measurement = sum / average_measurement;
  // Low Pass Filter
  filtered_measurement = 0.75 * filtered_measurement + 0.25 * measurement;
  measurement = filtered_measurement;

  if (measurement >= minLinearDistance && measurement <= maxLinearDistance)
  {
    error = Setpoint - measurement;
    float angle_pos = pid.computePID(error);

    String vars = pid.getVariables();

    Serial.print("*/" + String(measurement) + "," + String(error) + "," + String(angle_pos) + vars + "/*");
    Serial.println();

    servo.write(round(angle_pos));
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
  }
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