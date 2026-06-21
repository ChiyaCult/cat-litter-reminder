#include <unity.h>
#include <vector>
#include <string>
#include "connect_act_disconnect.h"

using domain::connectActDisconnect;

void setUp(void) {}
void tearDown(void) {}

void test_normal_case_calls_connect_action_disconnect_in_order(void) {
  std::vector<std::string> calls;

  bool result = connectActDisconnect(
      /*connect=*/[&]() { calls.push_back("connect"); return true; },
      /*action=*/[&]() { calls.push_back("action"); return true; },
      /*disconnect=*/[&]() { calls.push_back("disconnect"); });

  TEST_ASSERT_TRUE(result);
  TEST_ASSERT_EQUAL(3, (int)calls.size());
  TEST_ASSERT_EQUAL_STRING("connect", calls[0].c_str());
  TEST_ASSERT_EQUAL_STRING("action", calls[1].c_str());
  TEST_ASSERT_EQUAL_STRING("disconnect", calls[2].c_str());
}

void test_disconnect_still_runs_when_action_fails(void) {
  std::vector<std::string> calls;

  bool result = connectActDisconnect(
      /*connect=*/[&]() { calls.push_back("connect"); return true; },
      /*action=*/[&]() { calls.push_back("action"); return false; },
      /*disconnect=*/[&]() { calls.push_back("disconnect"); });

  TEST_ASSERT_FALSE(result);
  TEST_ASSERT_EQUAL(3, (int)calls.size());
  TEST_ASSERT_EQUAL_STRING("disconnect", calls[2].c_str());
}

void test_action_and_disconnect_never_run_when_connect_fails(void) {
  std::vector<std::string> calls;

  bool result = connectActDisconnect(
      /*connect=*/[&]() { calls.push_back("connect"); return false; },
      /*action=*/[&]() { calls.push_back("action"); return true; },
      /*disconnect=*/[&]() { calls.push_back("disconnect"); });

  TEST_ASSERT_FALSE(result);
  TEST_ASSERT_EQUAL(1, (int)calls.size());
  TEST_ASSERT_EQUAL_STRING("connect", calls[0].c_str());
}

int main(int argc, char **argv) {
  UNITY_BEGIN();
  RUN_TEST(test_normal_case_calls_connect_action_disconnect_in_order);
  RUN_TEST(test_disconnect_still_runs_when_action_fails);
  RUN_TEST(test_action_and_disconnect_never_run_when_connect_fails);
  return UNITY_END();
}
