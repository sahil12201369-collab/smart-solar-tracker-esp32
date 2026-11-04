// TEST 1 - LDR sensors
// Wiring: 3.3V -> LDR -> GPIO34 (or GPIO35) -> 10k -> GND
// Expected: covering an LDR with your finger makes its value DROP. Values range 0-4095.
// Open Serial Monitor at 115200 baud.

#define LDR_L_PIN 34
#define LDR_R_PIN 35

void setup() {
  Serial.begin(115200);
  analogSetAttenuation(ADC_11db);
}

void loop() {
  int l = analogRead(LDR_L_PIN);
  int r = analogRead(LDR_R_PIN);
  Serial.printf("LDR left = %4d   LDR right = %4d   diff = %5d\n", l, r, l - r);
  delay(300);
}
