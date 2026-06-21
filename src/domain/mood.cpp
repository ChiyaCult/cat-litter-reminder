#include "mood.h"

namespace domain {

Mood moodFromElapsed(time_t elapsedSeconds) {
    if (elapsedSeconds < 0) elapsedSeconds = 0;
    long long days = static_cast<long long>(elapsedSeconds) / 86400LL;
    if (days <= kHappyMaxDays)   return Mood::Happy;
    if (days <= kContentMaxDays) return Mood::Content;
    if (days <= kAnnoyedMaxDays) return Mood::Annoyed;
    return Mood::Disgusted;
}

} // namespace domain
