#include <unity.h>
#include "display_format.h"

using domain::formatLastCleaned;

void setUp(void) {}
void tearDown(void) {}

// Fixed reference points, UTC. 2026-06-18 12:00:00 UTC.
static const time_t kJun18Noon = 1781784000;

void test_same_day_shows_today(void) {
  // Cleaned at noon, "now" is 6 hours later, same calendar day.
  time_t now = kJun18Noon + 6 * 3600;
  TEST_ASSERT_EQUAL_STRING("Jun 18 (today)",
                            formatLastCleaned(kJun18Noon, now, 0).c_str());
}

void test_one_calendar_day_later_shows_yesterday(void) {
  time_t now = kJun18Noon + 24 * 3600;
  TEST_ASSERT_EQUAL_STRING("Jun 18 (yesterday)",
                            formatLastCleaned(kJun18Noon, now, 0).c_str());
}

void test_several_days_later_shows_n_days_ago(void) {
  time_t now = kJun18Noon + 3 * 24 * 3600;
  TEST_ASSERT_EQUAL_STRING("Jun 18 (3 days ago)",
                            formatLastCleaned(kJun18Noon, now, 0).c_str());
}

void test_midnight_boundary_crossed_with_little_elapsed_time(void) {
  // Cleaned at 23:50, "now" is only 20 minutes later but on the next
  // calendar day -- should read as "yesterday", not "today".
  time_t cleanedLateNight = kJun18Noon + 11 * 3600 + 50 * 60; // 23:50 Jun 18
  time_t now = cleanedLateNight + 20 * 60;                    // 00:10 Jun 19
  TEST_ASSERT_EQUAL_STRING(
      "Jun 18 (yesterday)",
      formatLastCleaned(cleanedLateNight, now, 0).c_str());
}

void test_same_instant_shows_today(void) {
  TEST_ASSERT_EQUAL_STRING(
      "Jun 18 (today)",
      formatLastCleaned(kJun18Noon, kJun18Noon, 0).c_str());
}

void test_utc_offset_shifts_calendar_day(void) {
  // 23:30 UTC on Jun 18 is already Jun 19 local time at UTC+1.
  time_t cleaned = kJun18Noon + 11 * 3600 + 30 * 60; // 23:30 UTC Jun 18
  long utcOffsetSeconds = 3600;                      // UTC+1
  TEST_ASSERT_EQUAL_STRING(
      "Jun 19 (today)",
      formatLastCleaned(cleaned, cleaned, utcOffsetSeconds).c_str());
}

void test_clock_moved_backward_does_not_go_negative(void) {
  // Defensive case: "now" is earlier than the recorded timestamp (e.g. a
  // re-sync corrected clock drift). Should not show a negative day count.
  time_t now = kJun18Noon - 3600;
  TEST_ASSERT_EQUAL_STRING("Jun 18 (today)",
                            formatLastCleaned(kJun18Noon, now, 0).c_str());
}

int main(int argc, char **argv) {
  UNITY_BEGIN();
  RUN_TEST(test_same_day_shows_today);
  RUN_TEST(test_one_calendar_day_later_shows_yesterday);
  RUN_TEST(test_several_days_later_shows_n_days_ago);
  RUN_TEST(test_midnight_boundary_crossed_with_little_elapsed_time);
  RUN_TEST(test_same_instant_shows_today);
  RUN_TEST(test_utc_offset_shifts_calendar_day);
  RUN_TEST(test_clock_moved_backward_does_not_go_negative);
  return UNITY_END();
}
