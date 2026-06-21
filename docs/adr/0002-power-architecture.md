# Power architecture: wall-powered v1, structured for future battery migration

The device will be wall-powered (USB) and always-on for v1 — no deep sleep, simplest possible firmware. Battery power with deep-sleep-between-presses is a plausible future upgrade for placement flexibility, but migrating to it later would normally require restructuring how WiFi/NTP/Home-Assistant calls happen (from "connect once at boot, assume always-connected" to "connect, act, disconnect on every wake").

**Decision**: Build v1 as continuously-powered with no deep sleep. To avoid a rewrite if battery + deep sleep is added later, structure all WiFi-dependent operations (Time Sync, future Home Assistant calls) as explicit connect → act → disconnect functions rather than code that assumes the connection is always up. Migrating to deep sleep later then mostly means calling those same functions after each wake, rather than re-architecting them.
