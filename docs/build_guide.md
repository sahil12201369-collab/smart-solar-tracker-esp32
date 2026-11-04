# Build guide (PCB + soldering, one day)

Solder one stage at a time and test it before starting the next. Use the sketches in `firmware/tests/` for each test.

## 0. Before soldering (30 min)

1. Install the Arduino IDE, then the **esp32 by Espressif** board package (board: *ESP32 Dev Module*).
2. Install the libraries **ESP32Servo** and **Adafruit INA219** (accept Adafruit BusIO).
3. Upload Blink / any test sketch to confirm the board and USB cable work. If it stays on *Connecting…*, hold the **BOOT** button while uploading.
4. Quickly test each part with jumper wires in the air (LDRs, servo, INA219) so a dead part is found before it is soldered.

## 1. Plan the PCB layout

Place everything on the board without solder and mark it with a pencil:

- ESP32 in the middle on **female headers**, INA219 on female headers beside it.
- Connectors (screw terminals or pin headers) along the edges for: LDR 1, LDR 2, servo, panel, battery/charger.
- One long **GND** line and one **3V3** line.

Perfboard: join holes with solder bridges or wire on the underside. Stripboard: cut the copper strips where two different signals share a strip.

## 2. Solder in stages

| Stage | Solder | Test with |
|---|---|---|
| A | ESP32 female headers, GND and 3V3 lines. Check no short before inserting the ESP32. | any sketch uploads |
| B | Both LDR dividers (3V3 → connector → GPIO34/35, 10 kΩ to GND) | `01_ldr_test` — covering an LDR lowers its value |
| C | Servo 3-pin header + capacitor | `02_servo_test` — servo sweeps without resets |
| D | INA219 female header (SDA/SCL/VCC/GND) + panel screw terminals | `03_ina219_test` — device at 0x40, voltage with light |
| E | Battery divider (2 × 10 kΩ → GPIO33). Keep the charger module off the PCB, connect it by wires. | `04_battery_adc_test` — matches the multimeter |

Soldering tips: iron at about 330–350 °C, heat pad and pin together for 2–3 s, then touch the solder to the joint. A good joint is shiny and cone shaped. Work in a ventilated place and keep the iron in its stand.

## 3. Mechanical assembly

1. Fix the servo firmly to the base.
2. Attach the panel to the servo horn so it tilts east-west.
3. Mount the two LDRs on the left and right edges of the panel, facing the same way as the panel.
4. Put a **1–2 cm cardboard divider** between the LDRs. This makes one LDR shaded when the light is off-axis.
5. Add the mirror last, beside the panel, so it does not shade the LDRs.
6. Leave slack in the wires so the moving panel does not pull on them.

## 4. Flash the main firmware

1. In `firmware/SolarTracker/`, copy `config.example.h` to `config.h` and enter your 2.4 GHz Wi-Fi name and password.
2. Open `SolarTracker.ino`, select *ESP32 Dev Module* and the COM port, then upload.
3. Open the Serial Monitor at 115200 baud. It prints `Dashboard: http://<ip>`. Open that address on a phone or laptop on the same Wi-Fi.
4. If Wi-Fi is not found within 15 s, connect to the hotspot **SolarTracker** (password `12345678`) and open `192.168.4.1`.

## 5. Calibrate and tune

| Symptom | Fix |
|---|---|
| Panel turns away from the light | Set `INVERT = true` in `SolarTracker.ino` |
| Servo jitters | Raise `DEADBAND` (for example 150 → 250) |
| Tracking too slow / too fast | Change `STEP_DEG` |
| Battery voltage differs from multimeter | `BAT_CAL = multimeter volts ÷ dashboard volts` |
| ESP32 restarts when the servo moves | Check the capacitor, common GND and use a good USB cable |
| `INA219 not found` | Check SDA/SCL and 3.3V/GND, try swapping SDA and SCL |
| Dashboard does not open | Same 2.4 GHz network for both devices; otherwise use the SolarTracker hotspot |

## 6. Demo

1. Place the panel in sunlight or under a bright lamp.
2. Set **MANUAL** with the slider at a tilted angle and note the power on the dashboard.
3. Press **Back to AUTO** and show that the power rises as the panel turns to the light.
4. Move the lamp slowly from left to right and show that the panel follows it.
5. Record the readings in the observation table of the report.
