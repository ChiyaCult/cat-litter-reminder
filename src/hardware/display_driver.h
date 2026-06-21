#pragma once
#include <string>
#include "domain/mood.h"

namespace hardware {

void initDisplay();

// Full refresh showing the formatted Last Cleaned Timestamp text alongside a
// placeholder cat illustration for the given Mood.
void showLastCleaned(const std::string &text, domain::Mood mood);

// Full refresh shown instead of a date whenever Time Sync hasn't succeeded
// yet, so a button press is never silently lost without explanation.
// See ADR-0001 and domain::canRecordCleaningEvent.
void showTimeNotSynced();

} // namespace hardware
