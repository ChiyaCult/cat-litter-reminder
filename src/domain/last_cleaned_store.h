#pragma once
#include <ctime>

namespace domain {

// Persists the Last Cleaned Timestamp. Only a single value is kept -- each
// Cleaning Event overwrites whatever was stored before (see CONTEXT.md).
//
// Real ESP32 firmware backs this with flash (NVS) -- see
// src/hardware/nvs_last_cleaned_store.h. Tests use an in-memory fake (see
// in_memory_last_cleaned_store.h) so recording logic can be verified without
// touching hardware.
class LastCleanedStore {
public:
  virtual ~LastCleanedStore() = default;

  // Returns true and sets outTimestamp if a Cleaning Event has ever been
  // recorded. Returns false if nothing has been recorded yet (e.g. a brand
  // new device).
  virtual bool get(time_t &outTimestamp) const = 0;

  // Persists timestamp as the new Last Cleaned Timestamp, overwriting any
  // previous value.
  virtual void set(time_t timestamp) = 0;
};

} // namespace domain
