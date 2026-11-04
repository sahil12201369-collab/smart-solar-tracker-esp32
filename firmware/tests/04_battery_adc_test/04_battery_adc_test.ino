// TEST 4 - Battery voltage divider
// Wiring: battery (+) -> 10k -> GPIO33 -> 10k -> GND   (both resistors must be equal)
// Expected: "battery" reading close to your multimeter reading of the battery.
// Calibration: BAT_CAL in SolarTracker.ino = multimeter volts / reading shown here.
// CAUTION: a Li-ion cell is up to 4.2 V, the divider brings GPIO33 to about 2.1 V (safe for the ESP32).

#define BAT_PIN 33

void setup() {
  Serial.begin(115200);
  analogSetAttenuation(ADC_11db);
}

void loop() {
  long s = 0;
  for (int i = 0; i < 16; i++) {
    s += analogRead(BAT_PIN);
    delayMicroseconds(200);
  }
  float raw = s / 16.0;
  float pinV = raw / 4095.0 * 3.3;
  float batV = pinV * 2.0;
  Serial.printf("raw = %.0f   pin = %.2f V   battery = %.2f V\n", raw, pinV, batV);
  delay(500);
}
