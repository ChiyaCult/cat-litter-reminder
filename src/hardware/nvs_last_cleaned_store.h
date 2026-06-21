#pragma once
#include "../domain/last_cleaned_store.h"

namespace hardware {

// Real implementation of domain::LastCleanedStore, backed by the ESP32's
// flash (NVS) via the Arduino Preferences library, so the Last Cleaned
// Timestamp survives power loss and reboots (see ADR-0001).
class NvsLastCleanedStore : public domain::LastCleanedStore {
public:
  bool get(time_t &outTimestamp) const override;
  void set(time_t timestamp) override;
};

} // namespace hardware
