#include "nvs_last_cleaned_store.h"
#include <Preferences.h>

namespace hardware {

namespace {
constexpr const char *kNamespace = "catlitter";
constexpr const char *kHasValueKey = "has_value";
constexpr const char *kTimestampKey = "last_cleaned";
} // namespace

bool NvsLastCleanedStore::get(time_t &outTimestamp) const {
  Preferences prefs;
  prefs.begin(kNamespace, /*readOnly=*/true);
  bool hasValue = prefs.getBool(kHasValueKey, false);
  if (hasValue) {
    outTimestamp = static_cast<time_t>(prefs.getULong64(kTimestampKey, 0));
  }
  prefs.end();
  return hasValue;
}

void NvsLastCleanedStore::set(time_t timestamp) {
  Preferences prefs;
  prefs.begin(kNamespace, /*readOnly=*/false);
  prefs.putULong64(kTimestampKey, static_cast<uint64_t>(timestamp));
  prefs.putBool(kHasValueKey, true);
  prefs.end();
}

} // namespace hardware
