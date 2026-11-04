/*
  Smart Solar Tracker - ESP32 firmware
  ------------------------------------
  Two LDRs (left / right) -> ESP32 ADC1 pins
  SG90 servo tilts the panel east-west towards the brighter side
  INA219 measures panel voltage / current / power
  Battery voltage measured through a 2 x 10k divider
  Live dashboard is served by the ESP32 itself (no cloud, no internet needed)

  Board : ESP32 Dev Module
  Libs  : ESP32Servo, Adafruit INA219 (+ Adafruit BusIO)
  Serial: 115200 baud

  Wi-Fi details go in config.h (copy config.example.h to config.h).
*/

#include <WiFi.h>
#include <WebServer.h>
#include <Wire.h>
#include <Adafruit_INA219.h>
#include <ESP32Servo.h>

#if __has_include("config.h")
#include "config.h"
#else
#include "config.example.h"
#endif
#include "dashboard.h"

// ---------------- Pins ----------------
#define LDR_L_PIN   34   // left LDR divider  (ADC1)
#define LDR_R_PIN   35   // right LDR divider (ADC1)
#define BAT_PIN     33   // battery divider   (ADC1)
#define SERVO_PIN   18
#define I2C_SDA     21
#define I2C_SCL     22

// ---------------- Tuning ----------------
const int   DEADBAND   = 150;     // ignore LDR differences smaller than this (0-4095 scale)
const int   STEP_DEG   = 2;       // servo step per control cycle
const int   MIN_ANGLE  = 20;      // mechanical limits of the panel
const int   MAX_ANGLE  = 160;
const bool  INVERT     = false;   // set true if the panel turns AWAY from the light
const float BAT_CAL    = 1.0;     // battery calibration: multimeter volts / dashboard volts
const unsigned long CONTROL_MS = 100;
const unsigned long PRINT_MS   = 1000;
const unsigned long WIFI_TIMEOUT_MS = 15000;

// ---------------- Globals ----------------
Adafruit_INA219 ina;
Servo servo;
WebServer server(80);

int   angle = 90;
bool  autoMode = true;
bool  inaOk = false;
int   ldrL = 0, ldrR = 0;
float busV = 0, curr_mA = 0, power_mW = 0, batV = 0;
int   batPct = 0;
unsigned long lastCtl = 0, lastPrint = 0;

int readAvg(int pin) {
  long s = 0;
  for (int i = 0; i < 16; i++) {
    s += analogRead(pin);
    delayMicroseconds(200);
  }
  return s / 16;
}

void handleData() {
  char buf[240];
  snprintf(buf, sizeof(buf),
           "{\"p\":%.1f,\"v\":%.2f,\"i\":%.1f,\"b\":%.2f,\"bp\":%d,\"l\":%d,\"r\":%d,\"a\":%d,\"auto\":%s}",
           power_mW, busV, curr_mA, batV, batPct, ldrL, ldrR, angle, autoMode ? "true" : "false");
  server.send(200, "application/json", buf);
}

void setupWifi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  unsigned long t = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - t < WIFI_TIMEOUT_MS) delay(300);

  if (WiFi.status() == WL_CONNECTED) {
    Serial.print("Dashboard: http://");
    Serial.println(WiFi.localIP());
  } else {
    WiFi.mode(WIFI_AP);
    WiFi.softAP(AP_SSID, AP_PASS);
    Serial.print("Wi-Fi not found. Hotspot '");
    Serial.print(AP_SSID);
    Serial.println("' started -> http://192.168.4.1");
  }
}

void setup() {
  Serial.begin(115200);
  analogSetAttenuation(ADC_11db);
  analogReadResolution(12);

  Wire.begin(I2C_SDA, I2C_SCL);
  inaOk = ina.begin();
  if (inaOk) {
    ina.setCalibration_16V_400mA();   // good resolution for a small 5-6 V panel (< 400 mA)
  } else {
    Serial.println("INA219 not found - check SDA/SCL/VCC/GND");
  }

  ESP32PWM::allocateTimer(0);
  servo.setPeriodHertz(50);
  servo.attach(SERVO_PIN, 500, 2400);
  servo.write(angle);

  setupWifi();

  server.on("/", []() { server.send_P(200, "text/html", PAGE); });
  server.on("/data", handleData);
  server.on("/set", []() {
    autoMode = false;
    angle = constrain(server.arg("angle").toInt(), MIN_ANGLE, MAX_ANGLE);
    servo.write(angle);
    server.send(200, "text/plain", "ok");
  });
  server.on("/mode", []() {
    autoMode = true;
    server.send(200, "text/plain", "ok");
  });
  server.begin();
}

void loop() {
  server.handleClient();

  if (millis() - lastCtl >= CONTROL_MS) {
    lastCtl = millis();

    ldrL = readAvg(LDR_L_PIN);
    ldrR = readAvg(LDR_R_PIN);

    if (inaOk) {
      busV     = ina.getBusVoltage_V();
      curr_mA  = max(0.0f, ina.getCurrent_mA());
      power_mW = busV * curr_mA;
    }

    batV   = readAvg(BAT_PIN) / 4095.0 * 3.3 * 2.0 * BAT_CAL;
    batPct = constrain((int)((batV - 3.2) / (4.2 - 3.2) * 100), 0, 100);

    if (autoMode) {
      int diff = ldrL - ldrR;
      if (INVERT) diff = -diff;
      if (abs(diff) > DEADBAND) {
        angle += (diff > 0) ? STEP_DEG : -STEP_DEG;
        angle = constrain(angle, MIN_ANGLE, MAX_ANGLE);
        servo.write(angle);
      }
    }
  }

  if (millis() - lastPrint >= PRINT_MS) {
    lastPrint = millis();
    Serial.printf("L=%d R=%d angle=%d mode=%s V=%.2f I=%.1fmA P=%.1fmW Bat=%.2fV (%d%%)\n",
                  ldrL, ldrR, angle, autoMode ? "AUTO" : "MANUAL", busV, curr_mA, power_mW, batV, batPct);
  }
}
