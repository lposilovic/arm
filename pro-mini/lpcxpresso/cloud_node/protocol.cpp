
#include "protocol.h"

#include "debug.h"
#include "protocol_util.h"
#include "protocol_tx.h"
#include "protocol_rx.h"
#include "esp8266.h"

namespace protocol {

static bool is_panic_mode;

// Delegate to protocol_gp so both protocol_tx and protocol_rx can
// access the panic flag.
void protocolPanic(const char* short_message) {
  if (!is_panic_mode) {
    debug.printf("Protocol panic: %s\n", short_message);
    esp8266::abortCurrentConnection();
    is_panic_mode = true;
  }
}

bool isPanicMode() {
  return is_panic_mode;
}

void setup() {
  protocol_tx::setup();
  protocol_rx::setup();
  resetForANewConnection();
}

void loop() {
  protocol_tx::loop();
  protocol_rx::loop();
}

void resetForANewConnection() {
  protocol_tx::resetForANewConnection();
  protocol_rx::resetForANewConnection();

  // Handshake counters.
  protocol_util::out_messages_counter = 0;
  protocol_util::in_messages_counter = 0;
}

void dumpInternalState() {
  debug.printf("protocol: msgs in=%d, out=%d, panic=%d\n",
      protocol_util::in_messages_counter, protocol_util::out_messages_counter, is_panic_mode);
  protocol_rx::dumpInternalState();
  protocol_tx::dumpInternalState();
}

}  // namespace protocol
