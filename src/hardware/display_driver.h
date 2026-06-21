#pragma once
#include <string>

namespace hardware {

void initDisplay();

// Full refresh showing the formatted Last Cleaned Timestamp text.
void showLastCleaned(const std::string &text);

// Full refresh shown instead of a date whenever Time Sync hasn't succeeded
// yet, so a button press is never silently lost without explanation.
// See ADR-0001 and domain::canRecordCleaningEvent.
void showTimeNotSynced();

} // namespace hardware
