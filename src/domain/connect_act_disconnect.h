#pragma once
#include <functional>

namespace domain {

using ConnectFn = std::function<bool()>;
using ActionFn = std::function<bool()>;
using DisconnectFn = std::function<void()>;

// Generic orchestration for any operation that needs a connection -- WiFi +
// NTP for Time Sync today, WiFi + MQTT for Home Assistant later: connect,
// run the action, then always disconnect, even if the action fails.
// Disconnect is never run if connect itself fails, since there's nothing to
// tear down.
//
// Built this way (rather than "connect once at boot, stay connected")
// so the same shape works if the device ever moves to battery + deep sleep,
// where connecting fresh on every wake is required. See ADR-0002.
//
// Returns the action's result; false if connect failed.
bool connectActDisconnect(const ConnectFn &connect, const ActionFn &action,
                           const DisconnectFn &disconnect);

} // namespace domain
