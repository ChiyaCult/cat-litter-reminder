#pragma once

namespace hardware {

// Attempts one Time Sync: connects to WiFi, configures NTP, waits for the
// system clock to report a plausible current time, then disconnects WiFi
// again -- regardless of success or failure. Built on top of
// domain::connectActDisconnect (see ADR-0002) so this same shape still works
// if the device ever migrates to battery + deep sleep.
//
// Returns true if the clock was successfully synced.
bool performTimeSync();

} // namespace hardware
