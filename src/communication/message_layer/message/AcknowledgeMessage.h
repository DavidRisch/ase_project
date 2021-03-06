#ifndef CROSSING_CONSOLES_ACKNOWLEDGEMESSAGE_H
#define CROSSING_CONSOLES_ACKNOWLEDGEMESSAGE_H

#include "../../ProtocolDefinition.h"
#include "Message.h"

namespace communication::message_layer {

/**
 * \brief Sent after a `Message` was received to notify the partner that it has arrived.
 */
class AcknowledgeMessage : public Message {
 public:
  static MessageType message_type;

  // Draft message
  explicit AcknowledgeMessage(ProtocolDefinition::sequence_t acknowledged_msg_sequence);

  // Received message
  AcknowledgeMessage(ProtocolDefinition::sequence_t acknowledged_msg_sequence, ProtocolDefinition::sequence_t sequence);

  [[nodiscard]] MessageType GetMessageType() const override;

  [[nodiscard]] ProtocolDefinition::sequence_t GetAcknowledgedMessageSequence() const;

 private:
  /// Sequence of the acknowledged message
  ProtocolDefinition::sequence_t acknowledged_msg_sequence;
};

}  // namespace communication::message_layer

#endif  // CROSSING_CONSOLES_ACKNOWLEDGEMESSAGE_H
