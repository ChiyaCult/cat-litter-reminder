#include "recording_gate.h"

namespace domain {

bool canRecordCleaningEvent(bool hasSyncedAtLeastOnce) {
  return hasSyncedAtLeastOnce;
}

} // namespace domain
