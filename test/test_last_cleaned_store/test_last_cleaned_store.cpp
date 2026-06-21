#include <unity.h>
#include "in_memory_last_cleaned_store.h"

using domain::InMemoryLastCleanedStore;

void setUp(void) {}
void tearDown(void) {}

void test_get_returns_false_when_nothing_recorded_yet(void) {
  InMemoryLastCleanedStore store;
  time_t out = 0;
  TEST_ASSERT_FALSE(store.get(out));
}

void test_set_then_get_returns_the_recorded_value(void) {
  InMemoryLastCleanedStore store;
  const time_t recorded = 1781784000;
  store.set(recorded);

  time_t out = 0;
  TEST_ASSERT_TRUE(store.get(out));
  TEST_ASSERT_EQUAL(recorded, out);
}

void test_second_set_overwrites_the_first(void) {
  InMemoryLastCleanedStore store;
  store.set(1781784000);
  store.set(1781870400); // one day later

  time_t out = 0;
  TEST_ASSERT_TRUE(store.get(out));
  TEST_ASSERT_EQUAL(1781870400, out);
}

int main(int argc, char **argv) {
  UNITY_BEGIN();
  RUN_TEST(test_get_returns_false_when_nothing_recorded_yet);
  RUN_TEST(test_set_then_get_returns_the_recorded_value);
  RUN_TEST(test_second_set_overwrites_the_first);
  return UNITY_END();
}
