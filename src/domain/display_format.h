#pragma once
#include <ctime>
#include <string>

namespace domain {

// Formats the Last Cleaned Timestamp for display, e.g. "Jun 18 (today)",
// "Jun 17 (yesterday)", "Jun 15 (3 days ago)".
//
// Day counting is calendar-based, not a raw 24-hour difference: a Cleaning
// Event at 11:50pm followed two hours later by "now" at 12:10am the next
// day is "yesterday" (1 day ago), even though under an hour has actually
// passed.
//
// utcOffsetSeconds shifts both timestamps to the device's local wall-clock
// time before computing the calendar date and day difference, without
// needing a timezone database on the device. Pass 0 to display in UTC.
std::string formatLastCleaned(time_t lastCleanedUtc, time_t nowUtc,
                               long utcOffsetSeconds);

} // namespace domain
