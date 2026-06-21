#pragma once
#include <ctime>

namespace domain {

enum class Mood { Happy, Content, Annoyed, Disgusted };

// Mood thresholds (inclusive upper bounds in calendar days).
// Adjust and reflash to tune the feel; see PRD-future-features.md.
constexpr int kHappyMaxDays   = 1;
constexpr int kContentMaxDays = 3;
constexpr int kAnnoyedMaxDays = 6;

// Pure function: elapsed seconds since Last Cleaned Timestamp → Mood.
// Uses calendar-day counting (floor division by 86400), consistent with
// formatLastCleaned. Negative values are clamped to 0 (clock moved backward).
Mood moodFromElapsed(time_t elapsedSeconds);

} // namespace domain
