#pragma once
#include "last_cleaned_store.h"

namespace domain {

// Test double for LastCleanedStore. Holds the value in memory only -- nothing
// survives past the object's lifetime. Used by unit tests; never used in the
// real firmware (which uses NvsLastCleanedStore instead).
class InMemoryLastCleanedStore : public LastCleanedStore {
public:
  bool get(time_t &outTimestamp) const override;
  void set(time_t timestamp) override;

private:
  bool hasValue_ = false;
  time_t value_ = 0;
};

} // namespace domain
