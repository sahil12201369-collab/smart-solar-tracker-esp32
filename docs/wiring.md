# Wiring

All grounds must be joined together (common GND). Overall picture: [block diagram](block_diagram.png).

## Connections

| Part | Connection |
|---|---|
| **LDR 1 (left)** | 3.3V → LDR → **GPIO34** → 10 kΩ → GND |
| **LDR 2 (right)** | 3.3V → LDR → **GPIO35** → 10 kΩ → GND |
| **SG90 servo** | Brown → GND, Red → **VIN (5V)**, Orange → **GPIO18** |
| **Servo capacitor** | 470–1000 µF across servo red (+, long leg) and brown (−) |
| **INA219 (control side)** | VCC → 3.3V, GND → GND, SDA → **GPIO21**, SCL → **GPIO22** |
| **INA219 (power side)** | Panel (+) → **VIN+**, **VIN−** → charger module **IN+** |
| **Panel (−)** | → charger module **IN−** and common GND (does not go through the INA219) |
| **Battery** | Battery (+) → charger **B+**, battery (−) → charger **B−** |
| **Battery voltage** | Battery (+) → 10 kΩ → **GPIO33** → 10 kΩ → GND |

The same table is in machine-readable form in [`hardware/pinout.csv`](../hardware/pinout.csv).

## Why these pins

- GPIO34, 35 and 33 are on **ADC1**, which keeps working while Wi-Fi is on (ADC2 does not). They are input-only pins, which is fine for sensing.
- The servo uses the **5V (VIN)** pin because 3.3V cannot supply its current. The capacitor stops the ESP32 from resetting when the servo moves.
- The INA219 measures the panel (+) line only; the panel (−) goes straight to ground.

## Checklist before applying power

1. Common GND: check panel (−), charger, servo, INA219, ESP32 and resistors with the multimeter continuity mode.
2. No short between 3V3 and GND (no beep in continuity mode) **before** the ESP32 is plugged into its sockets.
3. INA219: panel (+) enters **VIN+** and leaves **VIN−**. If reversed, the current reads negative.
4. Capacitor polarity: long leg towards the servo red wire.
5. All three 10 kΩ divider pairs use equal resistors (the battery divider must be exactly 1:1).
6. Charger module labels differ between boards (IN+/IN−, B+/B−, or +/−). Match them with the printed labels.
7. Battery polarity confirmed with the multimeter **before** it is connected.
8. Power the ESP32 from USB. Running it from the battery needs a 5V boost converter (not in the parts list).
