# Time sync and persistence strategy

The ESP32 has no real-time clock that survives power loss on its own. We need the device to reliably know "now" without adding an external RTC module (e.g. DS3231), to keep the bill of materials and wiring simple for v1.

**Decision**: Use WiFi + NTP for Time Sync on boot (and periodically). Persist only the single Last Cleaned Timestamp to flash (NVS) — not a continuously-running clock state. Cleaning Events are not recorded until at least one successful Time Sync has occurred; if the device hasn't synced yet (e.g. WiFi/router down), the button press is rejected and the display shows an unsynced state instead of silently recording an incorrect timestamp (e.g. Jan 1, 1970 — the ESP32's default unsynced clock value).

**Considered**: External battery-backed RTC module. Rejected for v1 — adds a part and wiring for a failure mode (no WiFi at the moment of a button press) we judged unlikely given the device will live near a router. Can be added later without changing the persistence model, since the Last Cleaned Timestamp is stored independently of whatever the live clock source is.
