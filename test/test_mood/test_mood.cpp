#include <unity.h>
#include "mood.h"

using domain::Mood;
using domain::moodFromElapsed;

void setUp(void) {}
void tearDown(void) {}

static constexpr time_t kOneDay  = 86400;

void test_zero_elapsed_is_happy(void) {
  TEST_ASSERT_EQUAL(static_cast<int>(Mood::Happy),
                    static_cast<int>(moodFromElapsed(0)));
}

void test_one_day_elapsed_is_happy(void) {
  TEST_ASSERT_EQUAL(static_cast<int>(Mood::Happy),
                    static_cast<int>(moodFromElapsed(1 * kOneDay)));
}

void test_two_days_elapsed_is_content(void) {
  TEST_ASSERT_EQUAL(static_cast<int>(Mood::Content),
                    static_cast<int>(moodFromElapsed(2 * kOneDay)));
}

void test_three_days_elapsed_is_content(void) {
  TEST_ASSERT_EQUAL(static_cast<int>(Mood::Content),
                    static_cast<int>(moodFromElapsed(3 * kOneDay)));
}

void test_four_days_elapsed_is_annoyed(void) {
  TEST_ASSERT_EQUAL(static_cast<int>(Mood::Annoyed),
                    static_cast<int>(moodFromElapsed(4 * kOneDay)));
}

void test_six_days_elapsed_is_annoyed(void) {
  TEST_ASSERT_EQUAL(static_cast<int>(Mood::Annoyed),
                    static_cast<int>(moodFromElapsed(6 * kOneDay)));
}

void test_seven_days_elapsed_is_disgusted(void) {
  TEST_ASSERT_EQUAL(static_cast<int>(Mood::Disgusted),
                    static_cast<int>(moodFromElapsed(7 * kOneDay)));
}

void test_long_elapsed_is_disgusted(void) {
  TEST_ASSERT_EQUAL(static_cast<int>(Mood::Disgusted),
                    static_cast<int>(moodFromElapsed(30 * kOneDay)));
}

int main(int argc, char **argv) {
  UNITY_BEGIN();
  RUN_TEST(test_zero_elapsed_is_happy);
  RUN_TEST(test_one_day_elapsed_is_happy);
  RUN_TEST(test_two_days_elapsed_is_content);
  RUN_TEST(test_three_days_elapsed_is_content);
  RUN_TEST(test_four_days_elapsed_is_annoyed);
  RUN_TEST(test_six_days_elapsed_is_annoyed);
  RUN_TEST(test_seven_days_elapsed_is_disgusted);
  RUN_TEST(test_long_elapsed_is_disgusted);
  return UNITY_END();
}
