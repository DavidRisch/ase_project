#include "PayloadMessage.h"

#include <utility>

PayloadMessage::PayloadMessage(address_t address, std::vector<uint8_t> payload, MessageMetaData meta_data)
    : Message(address, meta_data)
    , payload(std::move(payload)) {
}
PayloadMessage::PayloadMessage(address_t address, std::vector<uint8_t> payload)
    : Message(address)
    , payload(std::move(payload)) {
}

MessageType PayloadMessage::message_type = MessageType::PAYLOAD;

MessageType PayloadMessage::GetMessageType() const {
  return message_type;
}

std::vector<uint8_t> PayloadMessage::GetPayload() const {
  return payload;
}