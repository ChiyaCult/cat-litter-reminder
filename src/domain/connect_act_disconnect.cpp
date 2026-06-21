#include "connect_act_disconnect.h"

namespace domain {

bool connectActDisconnect(const ConnectFn &connect, const ActionFn &action,
                           const DisconnectFn &disconnect) {
  if (!connect()) {
    return false;
  }
  bool actionSucceeded = action();
  disconnect();
  return actionSucceeded;
}

} // namespace domain
