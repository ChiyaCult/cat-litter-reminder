#pragma once

namespace domain {

// Decides whether a button press should be accepted as a new Cleaning Event.
//
// A press is only accepted once the device has obtained correct time at
// least once via Time Sync. This prevents recording a bogus timestamp (e.g.
// the ESP32's default unsynced clock value of Jan 1, 1970) as a Cleaning
// Event -- see ADR-0001.
//
// hasSyncedAtLeastOnce: true once a successful Time Sync has completed since
// boot.
bool canRecordCleaningEvent(bool hasSyncedAtLeastOnce);

} // namespace domain
