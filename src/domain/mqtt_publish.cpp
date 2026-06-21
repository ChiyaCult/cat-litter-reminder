#include "mqtt_publish.h"
#include <cstdio>

namespace domain {

std::string mqttPayload(time_t lastCleanedUtc) {
  char buf[21]; // "2024-06-18T00:00:00Z\0"
  struct tm *t = gmtime(&lastCleanedUtc);
  strftime(buf, sizeof(buf), "%Y-%m-%dT%H:%M:%SZ", t);
  return buf;
}

ActionFn makeMqttPublishAction(time_t lastCleanedUtc,
                                const MqttPublishFn &publishFn) {
  return [lastCleanedUtc, publishFn]() -> bool {
    std::string payload = mqttPayload(lastCleanedUtc);
    return publishFn(kMqttTopic, payload.c_str());
  };
}

} // namespace domain
