# PRD: Cat Litter Reminder — MVP

## Problem Statement

In a multi-person household, it's hard to know whether anyone has cleaned the cat litter recently. Without a shared, visible record, people either re-clean litter that's already clean, or assume someone else handled it and it goes longer than intended without being cleaned.

## Solution

A small always-on device sits near the litter box with a button and an e-ink display. Any Household Member presses the button right after cleaning, recording a Cleaning Event. The display always shows the Last Cleaned Timestamp as a date plus how long ago that was, so anyone walking by can tell at a glance whether it's overdue — no app, no login, no phone needed.

## User Stories

1. As a Household Member, I want to press a single button right after cleaning the litter, so that the household has a shared record of when it was last done.
2. As a Household Member, I want the display to always show the last-cleaned date, so that I don't have to ask anyone or guess.
3. As a Household Member, I want the display to show how long ago the litter was cleaned (e.g. "3 days ago"), so that I can judge at a glance whether it's overdue without doing date math.
4. As a Household Member, I want the displayed information to update on its own each day, so that the "days ago" count stays accurate even if nobody presses the button.
5. As a Household Member, I want my button press to be ignored if the device doesn't yet know the correct time, so that a bad press never overwrites a correct record with a wrong date (e.g. Jan 1, 1970).
6. As a Household Member, I want some visible indication when the device doesn't know the correct time, so that I understand why my press didn't seem to register.
7. As the device owner, I want the Last Cleaned Timestamp to survive power loss or reboot, so that a power blip doesn't erase the household's record.
8. As the device owner, I want the device to get accurate time over WiFi automatically, so that I don't have to set the clock manually.
9. As the device owner, I want the device to keep working reliably while plugged into a wall outlet continuously, so that I don't have to think about batteries for the MVP.
10. As a future maintainer, I want the WiFi-dependent code (time sync, and later Home Assistant calls) written as discrete connect → act → disconnect operations, so that migrating to battery + deep sleep later doesn't require a rewrite.
11. As a future maintainer, I want the display-formatting and recording-decision logic to be hardware-independent, so that those rules can be tested without an ESP32 or e-ink panel attached.

## Implementation Decisions

- **Hardware**: ESP32 devkit + Adafruit ThinkInk 2.9" e-ink breakout (SSD1680 driver, to be confirmed against the physical board), one push button. Wall-powered (USB), always on — no deep sleep in the MVP.
- **Toolchain**: Arduino framework, built with PlatformIO. Display driven via the Adafruit_EPD library.
- **Time Sync**: WiFi connects on boot (and periodically thereafter) to obtain time via NTP. WiFi/NTP interactions are implemented as explicit connect → act → disconnect operations rather than an assumed always-on connection, per [ADR-0002](./adr/0002-power-architecture.md), to keep a future battery/deep-sleep migration from requiring a rewrite.
- **Recording gate**: A Cleaning Event is only recorded if at least one successful Time Sync has occurred since boot. If the device has not yet synced, a button press is rejected and the display shows an explicit "time not synced" state rather than a fallback or default date. See [ADR-0001](./adr/0001-time-sync-and-persistence-strategy.md).
- **Persistence**: Only the single Last Cleaned Timestamp is persisted, in flash (NVS) — not a history log, and not the live clock state. Each new Cleaning Event overwrites the previous value. Persistence is accessed through a small interface (get/set Last Cleaned Timestamp) so the recording-decision logic can be unit tested against a fake store, independent of the real NVS implementation.
- **Button**: Single short-press only triggers "record a Cleaning Event now." No long-press behavior, no debounce-driven secondary actions, and no per-Household-Member identification — the device treats all presses identically.
- **Display content**: Date plus relative duration, e.g. "Last cleaned: Jun 18 (3 days ago)." The relative-duration calculation is shared with the (future) Mood feature.
- **Display refresh cadence**: Full refresh once daily (e.g. at midnight) to keep the relative-duration value current, plus an immediate refresh whenever a Cleaning Event is recorded. No refresh tied to a live ticking clock.
- **Domain language**: Implementation should follow the terms defined in [CONTEXT.md](../CONTEXT.md) — Cleaning Event, Last Cleaned Timestamp, Household Member, Time Sync, Mood.

## Testing Decisions

- Good tests here exercise observable behavior — given a Last Cleaned Timestamp and a current time, or given a sync-state and a button-press event, what does the system produce/decide — not how the ESP32 talks to its peripherals.
- **Display-formatting function** (`Last Cleaned Timestamp + now → display string`): pure function, no hardware dependency. Unit test with a table of (timestamp, now) pairs covering same-day, "N days ago," and boundary-crossing-midnight cases.
- **Recording-decision function** (`sync-state + button-press event → accept/reject`): pure function. Unit test covering: press before any sync (reject), press after first successful sync (accept), press after a sync that later goes stale if/when periodic resync is added (out of scope for MVP, but the function should be structured so this case can be added later).
- **Persistence interface**: tested against a fake/in-memory store in unit tests; the real NVS-backed implementation is verified manually on-device (read survives reboot, write persists correctly) since flash I/O isn't practical to unit test off-device.
- **Connect → act → disconnect orchestration** (the wrapper used for Time Sync now, and future HA/MQTT calls, per [ADR-0002](./adr/0002-power-architecture.md)): the wrapper itself is pure orchestration logic — it takes `connect`, `action`, and `disconnect` as injectable functions, so it can be unit tested with fakes instead of real WiFi. Covers: normal case (connect → action → disconnect called in that order); action fails/throws (disconnect is still called — cleanup is guaranteed regardless of outcome); connect itself fails (action is never called). The real WiFi/NTP connect and disconnect implementations are verified manually on-device.
- These four are run as native PlatformIO unit tests (no hardware required), since the project has no existing test suite or prior art to follow yet — this establishes the pattern for future features (Home Assistant calls, Mood thresholds) to follow the same hardware-independent seam.
- E-ink rendering, button GPIO wiring, and WiFi/NTP network calls are not unit tested — verified manually on the physical device.

## Out of Scope

- Sending reminders via Home Assistant (planned as a follow-up feature).
- Cat mood animations on the display (planned as a follow-up feature).
- Battery power and deep sleep (explicitly deferred; MVP is wall-powered only, but structured to not block this later — see ADR-0002).
- Tracking which specific Household Member performed a Cleaning Event.
- A history log of past Cleaning Events (only the most recent is kept).
- Long-press or any secondary button function.
- External RTC hardware (e.g. DS3231) — deferred per ADR-0001; WiFi+NTP is the sole time source for MVP.

## Further Notes

- The exact Adafruit display variant is provisionally assumed to be the ThinkInk 2.9" breakout (SSD1680) based on the owner's recollection; pin mapping should be confirmed against the physical board/product page before wiring.
- Decisions here were reached via a structured design interview (see CONTEXT.md and ADRs 0001–0002 for the reasoning behind the time-sync, persistence, and power architecture choices).
