// TEST 2 - SG90 servo
// Wiring: servo signal (orange) -> GPIO18, red -> VIN (5V), brown -> GND
// Add a 470-1000 uF capacitor across servo red/brown. All GND must be common.
// Expected: the servo sweeps slowly between 20 and 160 degrees and back.
// If the ESP32 resets when the servo moves: add the capacitor / use a better USB cable.

#include <ESP32Servo.h>

#define SERVO_PIN 18
Servo servo;

void setup() {
  Serial.begin(115200);
  ESP32PWM::allocateTimer(0);
  servo.setPeriodHertz(50);
  servo.attach(SERVO_PIN, 500, 2400);
}

void loop() {
  for (int a = 20; a <= 160; a += 2) {
    servo.write(a);
    Serial.printf("angle = %d\n", a);
    delay(40);
  }
  for (int a = 160; a >= 20; a -= 2) {
    servo.write(a);
    Serial.printf("angle = %d\n", a);
    delay(40);
  }
}
