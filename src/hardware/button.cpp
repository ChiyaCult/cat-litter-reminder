#include "button.h"

namespace hardware {

Button::Button(uint8_t pin, unsigned long debounceMs)
    : pin_(pin), debounceMs_(debounceMs) {}

void Button::begin() { pinMode(pin_, INPUT_PULLUP); }

bool Button::update() {
  bool rawPressed = (digitalRead(pin_) == LOW);
  unsigned long now = millis();

  if (rawPressed != lastRawState_) {
    lastChangeMs_ = now;
    lastRawState_ = rawPressed;
  }

  bool justReleasedAfterPress = false;
  if ((now - lastChangeMs_) >= debounceMs_ && stableState_ != lastRawState_) {
    bool wasPressed = stableState_;
    stableState_ = lastRawState_;
    if (wasPressed && !stableState_) {
      justReleasedAfterPress = true;
    }
  }
  return justReleasedAfterPress;
}

} // namespace hardware
