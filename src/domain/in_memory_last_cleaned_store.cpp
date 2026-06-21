#include "in_memory_last_cleaned_store.h"

namespace domain {

bool InMemoryLastCleanedStore::get(time_t &outTimestamp) const {
  if (!hasValue_) {
    return false;
  }
  outTimestamp = value_;
  return true;
}

void InMemoryLastCleanedStore::set(time_t timestamp) {
  value_ = timestamp;
  hasValue_ = true;
}

} // namespace domain
