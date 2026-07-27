//motors.h
#ifndef MOTORS_H
#define MOTORS_H

// Core hardware control
void initMotors();
void setMotors(int leftSpeed, int rightSpeed);

// Movement directions
void moveForward(int speed);
void moveBackward(int speed);
void turnLeft(int speed);
void turnRight(int speed);
void stopMotors();

#endif // MOTORS_H