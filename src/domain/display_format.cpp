#include "display_format.h"
#include <cstdio>

namespace domain {

namespace {

long long floorDiv(long long a, long long b) {
  long long q = a / b;
  long long r = a % b;
  if (r != 0 && ((r < 0) != (b < 0))) {
    --q;
  }
  return q;
}

long long daysSinceEpoch(time_t shiftedUtc) {
  return floorDiv(static_cast<long long>(shiftedUtc), 86400LL);
}

} // namespace

std::string formatLastCleaned(time_t lastCleanedUtc, time_t nowUtc,
                               long utcOffsetSeconds) {
  time_t lastCleanedShifted = lastCleanedUtc + utcOffsetSeconds;
  time_t nowShifted = nowUtc + utcOffsetSeconds;

  long long dayDiff =
      daysSinceEpoch(nowShifted) - daysSinceEpoch(lastCleanedShifted);
  if (dayDiff < 0) {
    // Defensive: clock moved backward (e.g. a re-sync corrected drift).
    // Show "today" rather than a nonsensical negative count.
    dayDiff = 0;
  }

  struct tm dateParts;
  gmtime_r(&lastCleanedShifted, &dateParts);
  char dateBuf[16];
  strftime(dateBuf, sizeof(dateBuf), "%b %d", &dateParts);

  char phraseBuf[24];
  if (dayDiff == 0) {
    snprintf(phraseBuf, sizeof(phraseBuf), "today");
  } else if (dayDiff == 1) {
    snprintf(phraseBuf, sizeof(phraseBuf), "yesterday");
  } else {
    snprintf(phraseBuf, sizeof(phraseBuf), "%lld days ago", dayDiff);
  }

  char outBuf[48];
  snprintf(outBuf, sizeof(outBuf), "%s (%s)", dateBuf, phraseBuf);
  return std::string(outBuf);
}

} // namespace domain
