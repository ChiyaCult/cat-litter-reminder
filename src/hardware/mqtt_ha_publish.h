#pragma once
#include <ctime>

namespace hardware {

// Publishes the Last Cleaned Timestamp to Home Assistant over MQTT using the
// connect → act → disconnect pattern (see ADR-0002, ADR-0003). Connects to
// WiFi, connects to the MQTT broker (credentials from secrets.h), publishes
// a retained ISO 8601 payload to "cat-litter/last-cleaned", then disconnects.
//
// Returns true if the publish was acknowledged by the broker.
bool performMqttHaPublish(time_t lastCleanedUtc);

} // namespace hardware
