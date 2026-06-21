#include <unity.h>
#include <string>
#include <functional>
#include "mqtt_publish.h"

using domain::kMqttTopic;
using domain::mqttPayload;
using domain::makeMqttPublishAction;
using domain::MqttPublishFn;

void setUp(void) {}
void tearDown(void) {}

// 2024-06-18 00:00:00 UTC
static constexpr time_t kKnownTs = 1718668800;

void test_payload_formats_timestamp_as_iso8601(void) {
  TEST_ASSERT_EQUAL_STRING("2024-06-18T00:00:00Z", mqttPayload(kKnownTs).c_str());
}

void test_action_calls_publishfn_with_correct_topic_and_payload(void) {
  const char *capturedTopic = nullptr;
  std::string capturedPayload;

  auto action = makeMqttPublishAction(
      kKnownTs, [&](const char *topic, const char *payload) -> bool {
        capturedTopic = topic;
        capturedPayload = payload;
        return true;
      });

  bool result = action();

  TEST_ASSERT_TRUE(result);
  TEST_ASSERT_EQUAL_STRING(kMqttTopic, capturedTopic);
  TEST_ASSERT_EQUAL_STRING("2024-06-18T00:00:00Z", capturedPayload.c_str());
}

void test_action_propagates_publish_failure(void) {
  auto action = makeMqttPublishAction(
      kKnownTs, [](const char *, const char *) -> bool { return false; });

  TEST_ASSERT_FALSE(action());
}

int main(int argc, char **argv) {
  UNITY_BEGIN();
  RUN_TEST(test_payload_formats_timestamp_as_iso8601);
  RUN_TEST(test_action_calls_publishfn_with_correct_topic_and_payload);
  RUN_TEST(test_action_propagates_publish_failure);
  return UNITY_END();
}
