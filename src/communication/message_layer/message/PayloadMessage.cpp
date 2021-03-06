#include "PayloadMessage.h"

#include <utility>

using namespace communication;
using namespace communication::message_layer;

PayloadMessage::PayloadMessage(std::vector<uint8_t> payload, ProtocolDefinition::sequence_t sequence)
    : Message(sequence)
    , payload(std::move(payload)) {
}
PayloadMessage::PayloadMessage(std::vector<uint8_t> payload)
    : Message()
    , payload(std::move(payload)) {
}

MessageType PayloadMessage::message_type = MessageType::PAYLOAD;

MessageType PayloadMessage::GetMessageType() const {
  return message_type;
}

std::vector<uint8_t> PayloadMessage::GetPayload() const {
  return payload;
}
