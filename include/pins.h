#ifndef PINS_H
#define PINS_H

// --- ONBOARD RGB LED ---
#define RGB_LED_PIN  48

// --- 5-CHANNEL SENSOR PINS ---
#define SENS_FAR_L   7   
#define SENS_L       9   
#define SENS_C       4   
#define SENS_R       5   
#define SENS_FAR_R   6   

// --- NEW MOTOR DRIVER PINS (100% Conflict-Free) ---
#define PIN_ENA      16  // Speed Left (PWM) - Keep (Safe!)
#define PIN_IN1      17  // Dir Left 1       - Keep (Safe!)
#define PIN_IN2      18  // Dir Left 2       - Changed from 8 (JTAG) to 18 (Safe!)

#define PIN_ENB      15  // Speed Right (PWM)- Changed from 40 (MTDO) to 15 (Safe!)
#define PIN_IN3      3  // Dir Right 1      - Changed from 39 (MTCK) to 21 (Safe!)
#define PIN_IN4      10  // Dir Right 2      - Changed from 38 (FSPIWP) to 47 (Safe!)

#endif // PINS_H