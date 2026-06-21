# Cat Litter Reminder — MVP Firmware

ESP32 + Adafruit ThinkInk 2.9" e-ink display + one button. Press the button after cleaning the cat litter; the display always shows when it was last done. See `../CONTEXT.md` and `../docs/` for the design decisions (PRD, ADRs) behind this.

## Project layout

- `src/domain/` — hardware-independent logic (display formatting, the recording-decision gate, the persistence interface, the connect→act→disconnect wrapper). Compiles and tests on a regular computer, no ESP32 needed.
- `src/hardware/` — ESP32-specific code: NVS-backed persistence, button polling, the e-ink display driver, WiFi/NTP time sync. Only builds for the `esp32dev` environment.
- `src/main.cpp` — wires it all together (`setup()`/`loop()`).
- `test/` — native unit tests for everything in `src/domain/`.
- `include/secrets.h.example` — template for your WiFi credentials.

## Before you build

1. **Copy your WiFi credentials into place**:
   ```
   cp include/secrets.h.example include/secrets.h
   ```
   then edit `include/secrets.h` with your real SSID/password. This file is gitignored — never commit it.

2. **Display variant — confirmed.** This project uses the Adafruit 2.9" 296x128 Red/Black/White breakout (product #1028) in its **IL0373** chipset revision, which needs the `ThinkInk_290_Tricolor_Z10` initializer. That's the variant currently active (uncommented) in `src/hardware/display_driver.cpp`. The file also keeps the other chipset revisions Adafruit has sold under the same product line as commented-out alternatives, in case you ever swap the physical board — only `EPD_BLACK` is drawn either way, so no other code needs to change.

3. **Set your timezone offset.** In `src/main.cpp`, `UTC_OFFSET_SECONDS` defaults to `0` (UTC). Set it to your local offset in seconds (e.g. `3600` for UTC+1) so "today"/"yesterday" match your actual calendar day.

## Wiring (ESP32 DevKitC)

GPIO 6–11 are wired internally to the ESP32's flash chip — never use them, even though Adafruit's own Feather-oriented example sketches default to them. This project uses these pins instead:

| Signal | ESP32 GPIO |
|---|---|
| SPI SCK | 18 (hardware VSPI default) |
| SPI MISO | 19 (hardware VSPI default) |
| SPI MOSI | 23 (hardware VSPI default) |
| EPD_CS | 5 |
| EPD_DC | 25 |
| EPD_RESET | 26 |
| EPD_BUSY | 33 |
| SRAM_CS | 32 |
| Button | 27 (to GND, internal pull-up enabled in firmware) |

If you're using an ESP32-WROVER module (has onboard PSRAM), double check GPIO16/17 aren't in use elsewhere — not an issue with the pin set above, which avoids them deliberately.

## Building

Requires [PlatformIO](https://platformio.org/) (CLI or the VS Code extension).

```
# Run the hardware-independent unit tests (fast, no device needed):
pio test -e native

# Build and upload the real firmware to the ESP32:
pio run -e esp32dev -t upload

# Watch serial output:
pio device monitor -b 115200
```

## Status

The native unit test suite (`pio test -e native`) has been verified — all pass. The `esp32dev` firmware has been written against the documented Adafruit_EPD / ESP32 Arduino APIs but has **not yet been compiled or run on physical hardware** (no ESP32 toolchain available in the environment this was written in). Before relying on it: build it, flash it, and check the serial monitor and display carefully, especially the display variant selection in `display_driver.cpp` and the wiring table above.
