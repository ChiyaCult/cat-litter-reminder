#pragma once
#include <ctime>
#include <functional>
#include <string>
#include "connect_act_disconnect.h"

namespace domain {

constexpr char kMqttTopic[] = "cat-litter/last-cleaned";

// Pure function: formats the Last Cleaned Timestamp as an ISO 8601 UTC string,
// e.g. "2024-06-18T00:00:00Z". This is the MQTT payload published to HA.
std::string mqttPayload(time_t lastCleanedUtc);

// Type for the hardware publish callback injected at the call site.
// Returns true if the broker acknowledged the publish.
using MqttPublishFn = std::function<bool(const char *topic, const char *payload)>;

// Returns an ActionFn compatible with connectActDisconnect that publishes the
// Last Cleaned Timestamp to kMqttTopic using the given publishFn.
ActionFn makeMqttPublishAction(time_t lastCleanedUtc,
                                const MqttPublishFn &publishFn);

} // namespace domain
