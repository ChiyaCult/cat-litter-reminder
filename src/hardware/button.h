#pragma once
#include <Arduino.h>

namespace hardware {

// Polls a single push button wired to a digital input with INPUT_PULLUP
// (active LOW: pressed = pin reads LOW). Call update() once per loop()
// iteration; it returns true exactly once per completed, debounced short
// press (on release, not on press, to avoid double-firing on contact
// bounce).
class Button {
public:
  explicit Button(uint8_t pin, unsigned long debounceMs = 30);

  void begin();
  bool update();

private:
  uint8_t pin_;
  unsigned long debounceMs_;
  bool lastRawState_ = false; // true = pressed
  bool stableState_ = false;
  unsigned long lastChangeMs_ = 0;
};

} // namespace hardware
