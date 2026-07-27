#include <Arduino.h>
#include "pins.h"
#include "motors.h"
#include "network_ota.h"

// PID Parameters
float Kp = 30.0, Ki = 0.0, Kd = 5.0;
int baseSpeed = 100, maxSpeed = 190;
float error = 0, lastError = 0, integral = 0;

// Task handle for Network Task
TaskHandle_t NetworkTaskHandle = NULL;

// =======================================================
// CORE 0: Dedicated EXCLUSIVELY to Wi-Fi / OTA Processing
// =======================================================
void NetworkTask(void * pvParameters) {
  for (;;) {
    handleNetwork();
    // Yield 10ms to give system tasks and Watchdog breathing room on Core 0
    vTaskDelay(pdMS_TO_TICKS(10)); 
  }
}

void setup() {
  Serial.begin(115200);
  neopixelWrite(RGB_LED_PIN, 0, 0, 0); 

  // Initialize IR Sensor Inputs
  pinMode(SENS_FAR_L, INPUT);
  pinMode(SENS_L, INPUT);
  pinMode(SENS_C, INPUT);
  pinMode(SENS_R, INPUT);
  pinMode(SENS_FAR_R, INPUT);

  // 1. Initialize Motor Drivers FIRST
  initMotors();   
  stopMotors();

  // 2. Initiate Non-blocking Wi-Fi
  initNetwork();  

  // 3. Pin Network/OTA Handler strictly to CORE 0
  xTaskCreatePinnedToCore(
    NetworkTask,        /* Task function */
    "NetworkTask",      /* Name of task */
    8192,               /* Stack size (8KB) */
    NULL,               /* Parameter */
    1,                  /* Low priority to prevent blocking system tasks */
    &NetworkTaskHandle, /* Task handle */
    0                   /* CORE ID: 0 */
  );

  Serial.println("[SYSTEM] Core 0 (Network) and Core 1 (PID Motor Control) initialized.");
}

// =======================================================
// CORE 1: Dedicated EXCLUSIVELY to Real-Time PID & Motors
// =======================================================
void loop() {
  // 1. Read IR sensors (0 = Line Detected)
  int farL = digitalRead(SENS_FAR_L);
  int left = digitalRead(SENS_L);
  int cent = digitalRead(SENS_C);
  int right = digitalRead(SENS_R);
  int farR = digitalRead(SENS_FAR_R);

  // 2. Special Condition: All-Black (Intersection / Stop Line)
  if (farL == 0 && left == 0 && cent == 0 && right == 0 && farR == 0) {
    stopMotors();
    vTaskDelay(pdMS_TO_TICKS(1)); // Feed Watchdog
    return;
  }

  // 3. Position Error Mapping (-4.0 to +4.0)
  if      (cent == 0 && left == 1 && right == 1) error = 0.0;
  else if (left == 0 && cent == 0)                error = -1.0;
  else if (left == 0)                             error = -2.0;
  else if (farL == 0 && left == 0)                error = -3.0;
  else if (farL == 0)                             error = -4.0;
  else if (right == 0 && cent == 0)               error = 1.0;
  else if (right == 0)                            error = 2.0;
  else if (farR == 0 && right == 0)               error = 3.0;
  else if (farR == 0)                             error = 4.0;
  else {
    // Retain direction bias if line is temporarily lost
    error = (lastError >= 0) ? 3.0 : -3.0;
  }

  // 4. Compute PID output
  integral += error;
  integral = constrain(integral, -30, 30); // Prevent integral windup
  
  float derivative = error - lastError;
  float output = (Kp * error) + (Ki * integral) + (Kd * derivative);
  lastError = error;

  // 5. Differential Speed Output
  int leftMotorSpeed  = constrain(baseSpeed + output, 0, maxSpeed);
  int rightMotorSpeed = constrain(baseSpeed - output, 0, maxSpeed);

  setMotors(leftMotorSpeed, rightMotorSpeed);

  // Yield 1ms to feed Core 1's FreeRTOS Task Watchdog (1000 Hz control loop speed)
  vTaskDelay(pdMS_TO_TICKS(1)); 
}