#include "Change.h"

#include <cassert>

using namespace game;
using namespace game::networking;

Change::Change(ChangeType change_type)
    : change_type(change_type) {
  payload.push_back(static_cast<uint8_t>(change_type));
  assert(payload.size() == 1);
}

Change::Change(std::vector<uint8_t> bytes)
    : payload(bytes) {
  auto raw_change_type = bytes[0];
  assert(raw_change_type <= (uint8_t)ChangeType::HIGHEST_ELEMENT);
  change_type = static_cast<ChangeType>(raw_change_type);
}

ChangeType Change::GetChangeType() const {
  return change_type;
}

std::vector<uint8_t>::iterator Change::GetContentIterator() {
  auto iterator = payload.begin();
  ++iterator;
  return iterator;
}

bool Change::IsMovement() const {
  return (change_type == ChangeType::MOVE_UP || change_type == ChangeType::MOVE_DOWN ||
          change_type == ChangeType::MOVE_LEFT || change_type == ChangeType::MOVE_RIGHT);
}

bool Change::IsItemUsage() const {
  return (change_type == ChangeType::USE_UP || change_type == ChangeType::USE_DOWN ||
          change_type == ChangeType::USE_LEFT || change_type == ChangeType::USE_RIGHT ||
          change_type == ChangeType::USE_ITEM);
}
