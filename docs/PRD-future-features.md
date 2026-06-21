# PRD: Cat Litter Reminder — Home Assistant Reminders & Cat Mood Display

This PRD builds on the MVP (see [PRD-mvp.md](./PRD-mvp.md)) and assumes its persistence, Time Sync gating, and connect → act → disconnect WiFi pattern are already in place.

## Problem Statement

Even with the MVP working, a household member only learns the litter is overdue by walking over and looking at the device — nobody is proactively told. And the display only ever shows a date and a number of days, which doesn't convey "how bad it's gotten" in a way that grabs attention or is fun to glance at.

## Solution

Two additions on top of the MVP. First, the device publishes the Last Cleaned Timestamp to Home Assistant over MQTT whenever it's updated, so a Home Assistant automation can send a Reminder notification once the litter has gone too long without cleaning — fully configurable from the HA side, no reflashing needed. Second, the e-ink display shows a cat illustration whose Mood (Happy, Content, Annoyed, Disgusted) reflects how much time has passed since the last Cleaning Event, making the at-a-glance status more intuitive and a bit charming, not just a number.

## User Stories

1. As a Household Member, I want to receive a notification when the litter has gone too long without being cleaned, so that I don't have to remember to check the device myself.
2. As the device owner, I want to configure the Reminder threshold and notification behavior entirely within Home Assistant, so that I can tune it without reflashing the device.
3. As the device owner, I want the device to publish the Last Cleaned Timestamp to Home Assistant whenever a Cleaning Event occurs, so that Home Assistant always has accurate data to base Reminders on.
4. As the device owner, I want the published timestamp retained by MQTT, so that Home Assistant still has correct data after an HA restart, even if the device hasn't published anything recently.
5. As a future maintainer, I want the MQTT publish to reuse the existing connect → act → disconnect WiFi pattern, so that it doesn't introduce a new "always connected" assumption that would block a future battery migration.
6. As a Household Member, I do not want to be able to mark the litter as cleaned remotely via Home Assistant, so that the record only reflects a real, physical cleaning action taken at the box.
7. As the device owner, I want clear documentation of the MQTT topic and payload format, so that I can build the Home Assistant automation myself.
8. As a Household Member, I want the display to show a cat illustration whose mood reflects how overdue the cleaning is, so that the status is intuitive and a bit fun, not just a date.
9. As a Household Member, I want a clearly happy cat when the litter was recently cleaned, so that I get positive feedback for staying on top of it.
10. As a Household Member, I want a clearly unhappy/disgusted cat once the litter has gone a long time without cleaning, so that the urgency is obvious at a glance.
11. As the device owner, I want the Mood thresholds to be simple constants in firmware, so that I can adjust them easily if the 4 states/thresholds don't feel right after living with the device.
12. As the device owner, I want the Mood illustration to update on the same refresh cadence as the date/duration text (daily, plus on button press), so that no extra e-ink refresh cycles are introduced.
13. As a future maintainer, I want the Mood calculation to be a pure function (elapsed time → Mood state), so that the four thresholds can be unit tested without hardware.
14. As the device owner, I want placeholder cat artwork for the four Mood states to start with, so that I can validate the feature end-to-end before investing in final artwork.

## Implementation Decisions

- **Protocol**: Home Assistant integration via MQTT (e.g. PubSubClient), per [ADR-0003](./adr/0003-home-assistant-integration-architecture.md) — not HA's REST API.
- **Publish trigger & payload**: Device publishes the raw, absolute Last Cleaned Timestamp as a retained MQTT message to a single topic, only when a Cleaning Event occurs. No periodic heartbeat — Home Assistant computes elapsed time itself from the retained absolute value.
- **Decision boundary**: All Reminder logic (overdue threshold, notification routing, quiet hours) lives in a Home Assistant automation/template, not in firmware. Firmware stays a dumb publisher.
- **No remote trigger**: The device does not subscribe to any MQTT topic for remotely recording a Cleaning Event. The physical button remains the sole input. Deliberate scope cut, revisitable later.
- **Reuse of existing seam**: The MQTT publish is wrapped using the same connect → act → disconnect orchestration function built for Time Sync in the MVP ([ADR-0002](./adr/0002-power-architecture.md)) — connect to WiFi/broker, publish, disconnect. No new "always connected" code path.
- **Credentials**: MQTT broker host/port/credentials configured the same way WiFi credentials are in the MVP (kept out of source control), not hardcoded inline.
- **Mood calculation**: A pure function maps elapsed time since the Last Cleaned Timestamp to one of four Mood values, in increasing order of elapsed time: Happy (0–1 days), Content (2–3 days), Annoyed (4–6 days), Disgusted (7+ days). Thresholds are firmware constants, easy to adjust and reflash.
- **Mood display**: One static 1-bit bitmap illustration per Mood value, rendered alongside the existing date + relative-duration text, during the existing refresh cadence (daily + on button press). No animation, no additional refresh cycles.
- **Mood artwork**: Placeholder illustrations for the four states to start; final artwork can be swapped in later without any firmware logic changes (just new bitmap data).
- **Domain language**: Follows [CONTEXT.md](../CONTEXT.md) — Mood, Reminder, Cleaning Event, Last Cleaned Timestamp, Time Sync, Household Member.

## Testing Decisions

- **MQTT payload construction**: pure function (Last Cleaned Timestamp → payload string/format), unit tested without a real broker — same hardware-independent approach as the MVP's display-formatting function.
- **Mood-state calculation**: pure function (elapsed time → Mood), unit tested with a table of elapsed-time inputs spanning all four threshold boundaries (e.g. exactly 1 day, exactly 2 days, exactly 4 days, exactly 7 days, and values just below/above each).
- **MQTT publish operation**: reuses the already-tested connect → act → disconnect wrapper from the MVP — no new orchestration test needed. Only a thin check that the wrapped `action` is called with the correct topic and payload, verified with a fake `action`.
- **Not unit tested**: real MQTT broker connectivity and e-ink rendering of Mood bitmaps — verified manually on-device (e.g. against a local Mosquitto broker / test HA instance, and visually on the physical panel).

## Out of Scope

- Remote-triggered Cleaning Events via Home Assistant — button is the sole input (deliberate scope cut, see ADR-0003).
- Animated (multi-frame) Mood transitions — static illustration per Mood state only.
- Configurable Mood thresholds via Home Assistant or any runtime UI — thresholds are firmware constants for this phase.
- Final/polished cat artwork — placeholder art only in this phase.
- Battery power and deep sleep — still deferred; this PRD assumes the MVP's wall-powered architecture.
- Tracking which individual Household Member triggered, or was responsible for, a Reminder-worthy delay.

## Further Notes

- See [ADR-0003](./adr/0003-home-assistant-integration-architecture.md) for the reasoning behind the MQTT protocol choice, the retained-absolute-timestamp/no-heartbeat design, and the decision to let Home Assistant — not the device — own Reminder logic.
- Sourcing placeholder cat artwork for the four Mood states is an open task to pick up at implementation time.
