// TEST 3 - INA219 current / voltage sensor (also scans the I2C bus)
// Wiring: VCC -> 3.3V, GND -> GND, SDA -> GPIO21, SCL -> GPIO22
// Panel (+) -> VIN+ , VIN- -> charger IN+ , panel (-) -> GND
// Expected: an I2C device at 0x40, and V = about 4-6 V with current when the panel is in light.
// If "INA219 not found": check SDA/SCL (try swapping) and the 3.3V / GND connections.

#include <Wire.h>
#include <Adafruit_INA219.h>

Adafruit_INA219 ina;

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22);

  Serial.println("I2C scan:");
  for (uint8_t addr = 1; addr < 127; addr++) {
    Wire.beginTransmission(addr);
    if (Wire.endTransmission() == 0) Serial.printf("  device found at 0x%02X\n", addr);
  }

  if (!ina.begin()) {
    Serial.println("INA219 not found");
    while (true) delay(1000);
  }
  ina.setCalibration_16V_400mA();
}

void loop() {
  float v = ina.getBusVoltage_V();
  float i = ina.getCurrent_mA();
  Serial.printf("V = %.2f V   I = %.1f mA   P = %.1f mW\n", v, i, v * i);
  delay(500);
}
