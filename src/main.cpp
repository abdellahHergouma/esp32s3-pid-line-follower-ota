#include <Arduino.h>
#include "pins.h"
#include "motors.h"
#include "network_ota.h"

// 1. Toned down speeds for better traction and reading accuracy
const int BASE_SPEED = 180;  // Dropped from 160
const int TURN_SPEED = 120
;  // Dropped from 180
const int SLOW_SPEED = 180;   // Dropped from 70

unsigned long lastLogTime = 0;

// Track the last known direction to recover when "Lost Line" happens
enum LastSeen { LEFT, RIGHT, CENTER };
LastSeen lastDirection = CENTER;

void setup() {
  Serial.begin(115200);
  neopixelWrite(RGB_LED_PIN, 0, 0, 0); 

  pinMode(SENS_FAR_L, INPUT);
  pinMode(SENS_L, INPUT);
  pinMode(SENS_C, INPUT);
  pinMode(SENS_R, INPUT);
  pinMode(SENS_FAR_R, INPUT);

  initMotors();   
  initNetwork();  // Ensure this function doesn't contain a "while(!client)" loop!
}

void loop() {
  handleNetwork(); 

  int farL = digitalRead(SENS_FAR_L);
  int left = digitalRead(SENS_L);
  int cent = digitalRead(SENS_C);
  int right = digitalRead(SENS_R);
  int farR = digitalRead(SENS_FAR_R);

  // Print raw sensor values and memory state to Serial Monitor
  // Format: [FarL, Left, Center, Right, FarR] | LastDir: X | State: Y
  Serial.printf("[%d, %d, %d, %d, %d] | LastDir: %d | Action: ", farL, left, cent, right, farR, lastDirection);

  // Line-Following Logic Engine
  if (farL == 0 && left == 0 && cent == 0 && right == 0 && farR == 0) {
    stopMotors();
    Serial.println("All Black Stop");
  }
  else if (farL == 0) {
    turnLeft(TURN_SPEED);
    lastDirection = LEFT;
    Serial.println("Hard Left");
  }
  else if (farR == 0) {
    turnRight(TURN_SPEED);
    lastDirection = RIGHT;
    Serial.println("Hard Right");
  }
  else if (left == 0) {
    setMotors(SLOW_SPEED, TURN_SPEED);
    lastDirection = LEFT;
    Serial.println("Soft Left");
  }
  else if (right == 0) {
    setMotors(TURN_SPEED, SLOW_SPEED);
    lastDirection = RIGHT;
    Serial.println("Soft Right");
  }
  else if (cent == 0) {
    moveForward(BASE_SPEED);
    lastDirection = CENTER;
    Serial.println("Forward");
  }
  else {
    if (lastDirection == LEFT) {
      turnLeft(TURN_SPEED);
      Serial.println("Recovering Left");
    } else if (lastDirection == RIGHT) {
      turnRight(TURN_SPEED);
      Serial.println("Recovering Right");
    } else {
      stopMotors();
      Serial.println("Dead Stop (All White)");
    }
  }

  delay(50); // Small stability delay so it doesn't flood the monitor too fast
}