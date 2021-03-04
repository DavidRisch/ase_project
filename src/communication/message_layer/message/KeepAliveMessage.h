#ifndef CROSSING_CONSOLES_KEEPALIVEMESSAGE_H
#define CROSSING_CONSOLES_KEEPALIVEMESSAGE_H

#include "Message.h"

namespace communication {
namespace message_layer {

class KeepAliveMessage : public Message {
 public:
  static MessageType message_type;

  // Draft message
  explicit KeepAliveMessage(address_t address);

  // Received message
  KeepAliveMessage(address_t address, MessageMetaData meta_data);

  [[nodiscard]] MessageType GetMessageType() const override;
};

}  // namespace message_layer
}  // namespace communication

#endif  // CROSSING_CONSOLES_KEEPALIVEMESSAGE_H