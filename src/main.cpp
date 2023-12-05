#include <Arduino.h>
#include <Servo.h>
#include <HCSR04.h>
#include <PID_2.h>

#define TRIG_PIN 12
#define ECHO_PIN 11
float measurement;

#define servoPin 9
int minServoValue = 480;
int maxServoValue = 2500;
Servo servo;

void set_servo();
void home();
void check_home();

int minServoAngle = 45;
int maxServoAngle = 110;
int HomeServoAngle = 90;

double maxLinearDistance = 45;
double minLinearDistance = 4;
double Setpoint = 18.25;
double timeout = 500;
double average_measurement = 10;
float error, momentum;

int button_pin = 6;

UltraSonicDistanceSensor sensor(TRIG_PIN, ECHO_PIN);

const double k_P = 10, k_I = 0.50, k_D = 5.0, T = 0.005;

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

void loop()
{
  long sum = 0;
  for (int i = 0; i < average_measurement; i++)
  {
    measurement = sensor.measureDistanceCm();
    sum += measurement;
  }
  measurement = sum / average_measurement;

  if (measurement >= minLinearDistance && measurement <= maxLinearDistance)
  {
    error = Setpoint - measurement;
    float angle_pos = pid.computePID(error);
    check_home();

    String vars = pid.getVariables();

    Serial.print("*/");
    Serial.print(measurement);
    Serial.print(",");
    Serial.print(error);
    Serial.print(",");
    Serial.print(angle_pos);
    Serial.print(vars);
    Serial.println("/*");

    servo.write(round(angle_pos));
    check_home();
  }

  check_home();

  Serial.print("*/");
  Serial.print(measurement);
  Serial.print(", 0.0, 0.0, 0.0, 0.0");
  Serial.println("/*");

  if (measurement == -1){
    home();
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