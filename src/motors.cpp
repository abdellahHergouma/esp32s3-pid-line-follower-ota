#include <Arduino.h>
#include "pins.h"
#include "motors.h"

void initMotors() {
  pinMode(PIN_ENA, OUTPUT);
  pinMode(PIN_IN1, OUTPUT);
  pinMode(PIN_IN2, OUTPUT);
  pinMode(PIN_ENB, OUTPUT);
  pinMode(PIN_IN3, OUTPUT);
  pinMode(PIN_IN4, OUTPUT);
}

void setMotors(int leftSpeed, int rightSpeed) {
  // Left Motor Control
  if (leftSpeed >= 0) {
    digitalWrite(PIN_IN1, HIGH);
    digitalWrite(PIN_IN2, LOW);
    analogWrite(PIN_ENA, leftSpeed);
  } else {
    digitalWrite(PIN_IN1, LOW);
    digitalWrite(PIN_IN2, HIGH);
    analogWrite(PIN_ENA, -leftSpeed);
  }

  // Right Motor Control
  if (rightSpeed >= 0) {
    digitalWrite(PIN_IN3, HIGH);
    digitalWrite(PIN_IN4, LOW);
    analogWrite(PIN_ENB, rightSpeed);
  } else {
    digitalWrite(PIN_IN3, LOW);
    digitalWrite(PIN_IN4, HIGH);
    analogWrite(PIN_ENB, -rightSpeed);
  }
}

void moveForward(int speed)  { setMotors(speed, speed); }
void moveBackward(int speed) { setMotors(-speed, -speed); }
void turnLeft(int speed)     { setMotors(-speed, speed); }
void turnRight(int speed)    { setMotors(speed, -speed); }
void stopMotors()            { setMotors(0, 0); }