#include "wifi_time_sync.h"
#include "../domain/connect_act_disconnect.h"
#include <WiFi.h>
#include <time.h>
#include "secrets.h" // WIFI_SSID, WIFI_PASSWORD -- copy secrets.h.example to create this

namespace hardware {

namespace {

constexpr const char *kNtpServer = "pool.ntp.org";
// The device's internal clock is kept in UTC; UTC_OFFSET_SECONDS in main.cpp
// is applied only when formatting for display.
constexpr long kGmtOffsetSec = 0;
constexpr int kDaylightOffsetSec = 0;
constexpr unsigned long kWifiConnectTimeoutMs = 15000;
constexpr unsigned long kNtpSyncTimeoutMs = 10000;
// A freshly booted, unsynced ESP32 clock reads near epoch 0 (1970). Once
// NTP has actually responded, the clock will read a plausible "modern"
// time -- this is how we detect that sync succeeded.
constexpr time_t kPlausibleMinimumEpoch = 1700000000; // ~November 2023

bool connectWifi() {
  Serial.print("[wifi] connecting to \"");
  Serial.print(WIFI_SSID);
  Serial.println("\"...");

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  unsigned long start = millis();
  while (WiFi.status() != WL_CONNECTED) {
    if (millis() - start > kWifiConnectTimeoutMs) {
      Serial.println("[wifi] connect timed out");
      return false;
    }
    delay(100);
  }
  Serial.print("[wifi] connected, IP ");
  Serial.println(WiFi.localIP());
  return true;
}

bool syncNtp() {
  Serial.println("[ntp] requesting time...");
  configTime(kGmtOffsetSec, kDaylightOffsetSec, kNtpServer);
  unsigned long start = millis();
  time_t now = time(nullptr);
  while (now < kPlausibleMinimumEpoch) {
    if (millis() - start > kNtpSyncTimeoutMs) {
      Serial.println("[ntp] sync timed out");
      return false;
    }
    delay(200);
    now = time(nullptr);
  }
  Serial.print("[ntp] synced, current UTC epoch = ");
  Serial.println((long)now);
  return true;
}

void disconnectWifi() {
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
  Serial.println("[wifi] disconnected");
}

} // namespace

bool performTimeSync() {
  return domain::connectActDisconnect(connectWifi, syncNtp, disconnectWifi);
}

} // namespace hardware
