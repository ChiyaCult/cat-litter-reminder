#include <Arduino.h>
#include <time.h>
#include <string>

#include "domain/display_format.h"
#include "domain/recording_gate.h"
#include "hardware/button.h"
#include "hardware/display_driver.h"
#include "hardware/nvs_last_cleaned_store.h"
#include "hardware/wifi_time_sync.h"

namespace {

// The device's internal clock is kept in UTC. This offset (in seconds) is
// applied only when formatting dates for display and counting calendar
// days -- see domain::formatLastCleaned. Adjust for your timezone, e.g.
// -18000 for US Eastern Standard Time, 3600 for UTC+1. This does not
// observe daylight saving automatically; update it manually if needed.
constexpr long UTC_OFFSET_SECONDS = 0;

constexpr uint8_t BUTTON_PIN = 27;
constexpr unsigned long DAILY_REFRESH_INTERVAL_MS = 24UL * 60 * 60 * 1000;
// While unsynced, retry Time Sync this often until it succeeds.
constexpr unsigned long SYNC_RETRY_INTERVAL_MS = 5UL * 60 * 1000;

hardware::NvsLastCleanedStore store;
hardware::Button button(BUTTON_PIN);

bool hasSyncedAtLeastOnce = false;
unsigned long lastSyncAttemptMs = 0;
unsigned long lastDisplayRefreshMs = 0;

void refreshDisplay() {
  if (!hasSyncedAtLeastOnce) {
    hardware::showTimeNotSynced();
    lastDisplayRefreshMs = millis();
    return;
  }

  time_t lastCleaned;
  if (store.get(lastCleaned)) {
    time_t now = time(nullptr);
    std::string text =
        domain::formatLastCleaned(lastCleaned, now, UTC_OFFSET_SECONDS);
    hardware::showLastCleaned(text);
  } else {
    hardware::showLastCleaned("never recorded yet");
  }
  lastDisplayRefreshMs = millis();
}

void attemptTimeSync() {
  lastSyncAttemptMs = millis();
  bool syncedNow = hardware::performTimeSync();
  if (syncedNow) {
    bool justSynced = !hasSyncedAtLeastOnce;
    hasSyncedAtLeastOnce = true;
    if (justSynced) {
      Serial.println("[main] time sync succeeded, leaving unsynced screen");
      refreshDisplay(); // leave the "not synced" screen as soon as we can
    } else {
      Serial.println("[main] periodic re-sync succeeded");
    }
  } else {
    Serial.println("[main] time sync failed, will retry later");
  }
}

} // namespace

void setup() {
  Serial.begin(115200);
  delay(500); // give the serial monitor a moment to attach after boot
  Serial.println("[main] booting cat litter reminder...");

  button.begin();
  hardware::initDisplay();
  hardware::showTimeNotSynced();
  Serial.println("[main] display initialized, showing 'time not synced'");

  attemptTimeSync();
  if (hasSyncedAtLeastOnce) {
    refreshDisplay();
  }
}

void loop() {
  if (button.update()) {
    Serial.println("[main] button press detected");
    // A press only records a Cleaning Event if Time Sync has succeeded at
    // least once -- otherwise we'd risk recording a bogus pre-1970
    // timestamp. See ADR-0001 and domain::canRecordCleaningEvent. If
    // rejected, we deliberately do nothing else: the "time not synced"
    // screen already explains why the press didn't register.
    if (domain::canRecordCleaningEvent(hasSyncedAtLeastOnce)) {
      store.set(time(nullptr));
      Serial.println("[main] cleaning event recorded");
      refreshDisplay();
    } else {
      Serial.println("[main] press rejected -- time not synced yet");
    }
  }

  if (!hasSyncedAtLeastOnce &&
      (millis() - lastSyncAttemptMs) >= SYNC_RETRY_INTERVAL_MS) {
    Serial.println("[main] retrying time sync...");
    attemptTimeSync();
  }

  if ((millis() - lastDisplayRefreshMs) >= DAILY_REFRESH_INTERVAL_MS) {
    // Re-sync once a day too, so the clock doesn't drift indefinitely.
    Serial.println("[main] daily refresh triggered");
    attemptTimeSync();
    refreshDisplay();
  }
}
