#include <unity.h>
#include "recording_gate.h"

using domain::canRecordCleaningEvent;

void setUp(void) {}
void tearDown(void) {}

void test_press_rejected_before_any_sync(void) {
  TEST_ASSERT_FALSE(canRecordCleaningEvent(/*hasSyncedAtLeastOnce=*/false));
}

void test_press_accepted_after_first_successful_sync(void) {
  TEST_ASSERT_TRUE(canRecordCleaningEvent(/*hasSyncedAtLeastOnce=*/true));
}

int main(int argc, char **argv) {
  UNITY_BEGIN();
  RUN_TEST(test_press_rejected_before_any_sync);
  RUN_TEST(test_press_accepted_after_first_successful_sync);
  return UNITY_END();
}
