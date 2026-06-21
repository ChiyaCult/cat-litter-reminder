#include "display_driver.h"
#include "Adafruit_ThinkInk.h"
#include <SPI.h>

namespace hardware {

namespace {

// ---- Wiring (ESP32 DevKitC + ThinkInk 2.9" breakout) ----
// SPI uses the ESP32's default hardware VSPI pins (SCK=18, MISO=19,
// MOSI=23) -- call SPI.begin() with no arguments to get these.
//
// IMPORTANT: GPIO 6-11 are wired internally to the ESP32's SPI flash chip
// and must NEVER be used for the display/button, even though Adafruit's
// own Feather-oriented example sketches default to them. The pins below
// are safe, general-purpose GPIOs on a standard ESP32 DevKitC.
constexpr int EPD_CS = 5; // also the default VSPI SS pin
constexpr int EPD_DC = 25;
constexpr int EPD_RESET = 26;
constexpr int EPD_BUSY = 33;
constexpr int SRAM_CS = 32;

// ---- Display variant ----
// Adafruit has sold a few different chipset revisions of the 2.9" 296x128
// breakout under the same product line. Confirm which one you actually
// have (check the board's label or your order/product page) and uncomment
// exactly one whole block below -- both the display object AND the
// matching kDisplayMode need to change together. Everything else in this
// file draws using EPD_BLACK only, so it works unchanged regardless of
// which variant is correct.

// Variant A (default): 2.9" 296x128, 4-level grayscale (used here as plain
// monochrome), SSD1680 chip -- most likely match if your board's chip is
// SSD1680 and it is NOT the red/black/white tri-color version.
ThinkInk_290_Grayscale4_EAAMFGN display(EPD_DC, EPD_RESET, EPD_CS, SRAM_CS,
                                         EPD_BUSY, &SPI);
constexpr uint8_t kDisplayMode = THINKINK_MONO;

// Variant B: 2.9" 296x128 Tri-Color (Black/White/Red) breakout, SSD1680
// chip -- Adafruit product #1028 as sold since the June 2025 SSD1680
// revision. Uncomment this pair (and comment out Variant A above) if your
// board is the red/black/white variant.
// ThinkInk_290_Tricolor_Z94 display(EPD_DC, EPD_RESET, EPD_CS, SRAM_CS,
//                                    EPD_BUSY, &SPI);
// constexpr uint8_t kDisplayMode = THINKINK_TRICOLOR;

// Variant C: 2.9" 296x128 monochrome, UC8151D chip (older flexible
// display). Uncomment this pair (and comment out Variant A above) if so.
// ThinkInk_290_Mono_M06 display(EPD_DC, EPD_RESET, EPD_CS, SRAM_CS,
//                                EPD_BUSY, &SPI);
// constexpr uint8_t kDisplayMode = THINKINK_MONO;

constexpr int16_t kTextX = 8;
constexpr int16_t kLabelY = 40;
constexpr int16_t kValueY = 64;

} // namespace

void initDisplay() {
  SPI.begin(); // default VSPI pins: SCK=18, MISO=19, MOSI=23
  display.begin(kDisplayMode);
}

void showLastCleaned(const std::string &text) {
  display.clearBuffer();
  display.setTextColor(EPD_BLACK);
  display.setTextSize(2);
  display.setCursor(kTextX, kLabelY);
  display.print("Last cleaned:");
  display.setCursor(kTextX, kValueY);
  display.print(text.c_str());
  display.display();
}

void showTimeNotSynced() {
  display.clearBuffer();
  display.setTextColor(EPD_BLACK);
  display.setTextSize(2);
  display.setCursor(kTextX, kLabelY);
  display.print("Time not synced");
  display.setCursor(kTextX, kValueY);
  display.print("Waiting for WiFi...");
  display.display();
}

} // namespace hardware
