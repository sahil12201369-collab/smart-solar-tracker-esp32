// Copy this file to "config.h" in the same folder and put your real Wi-Fi details there.
// config.h is listed in .gitignore, so your password is NOT uploaded to GitHub.
// Use a 2.4 GHz network (the ESP32 does not support 5 GHz). A phone hotspot works well.
#pragma once

#define WIFI_SSID "YOUR_WIFI_NAME"
#define WIFI_PASS "YOUR_WIFI_PASSWORD"

// If the Wi-Fi above cannot be reached within 15 s, the ESP32 starts its own hotspot:
#define AP_SSID   "SolarTracker"
#define AP_PASS   "12345678"      // minimum 8 characters
