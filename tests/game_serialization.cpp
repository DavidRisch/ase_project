#include <gtest/gtest.h>

#include <chrono>
#include <cstring>

#include "../src/game/common/Color.h"
#include "../src/game/networking/SerializationUtils.h"
#include "../src/game/world/World.h"

using namespace game;
using namespace game::common;
using namespace game::world;

template <typename T>
T serialize_and_deserialize(const T &original) {
  std::vector<uint8_t> encoded;
  original.Serialize(encoded);

  EXPECT_GT(encoded.size(), 0);

  auto iterator = encoded.begin();
  auto deserialized = T::Deserialize(iterator);
  EXPECT_EQ(iterator, encoded.end());

  return deserialized;
}

TEST(GameSerialization, Position) {
  Position original(12, 23);

  auto deserialized = serialize_and_deserialize(original);

  EXPECT_EQ(original, deserialized);
}

TEST(GameSerialization, Wall) {
  Wall original(Position(34, 56));

  auto deserialized = serialize_and_deserialize(original);

  EXPECT_EQ(original.position, deserialized.position);
  EXPECT_EQ(original.type, deserialized.type);
}

TEST(GameSerialization, World) {
  World original(Position(34, 56));

  original.AddWall(Position(1, 2));
  original.AddWall(Position(3, 4));
  original.AddWall(Position(5, 6));

  auto deserialized = serialize_and_deserialize(original);

  EXPECT_EQ(original.size, deserialized.size);

  ASSERT_EQ(original.walls.size(), deserialized.walls.size());
  for (const auto &pair_original : original.walls) {
    auto wall_original = pair_original.second;
    int identical_count = 0;
    for (const auto &pair_deserialized : deserialized.walls) {
      auto wall_deserialized = pair_deserialized.second;
      if (wall_original.position == wall_deserialized.position) {
        identical_count++;
      }
    }
    EXPECT_EQ(identical_count, 1);
  }
}

TEST(GameSerialization, Player) {
  Player original("ABCD", Position(34, 56));

  original.IncreaseScore(4);
  original.health = 3;

  auto deserialized = serialize_and_deserialize(original);

  EXPECT_EQ(original.name, deserialized.name);
  EXPECT_EQ(original.color, deserialized.color);
  EXPECT_EQ(original.health, deserialized.health);
  EXPECT_EQ(original.direction, deserialized.direction);
  EXPECT_EQ(original.GetScore(), deserialized.GetScore());
  EXPECT_EQ(original.position, deserialized.position);
}

TEST(GameSerialization, Utils) {
  int a_original = 123;
  auto b_original = std::chrono::milliseconds(456);
  long long c_original = 123456789;

  std::vector<uint8_t> encoded;

  networking::SerializationUtils::SerializeObject(a_original, encoded);
  networking::SerializationUtils::SerializeObject(b_original, encoded);
  networking::SerializationUtils::SerializeObject(c_original, encoded);

  auto it = encoded.begin();

  auto a_decoded = networking::SerializationUtils::DeserializeObject<decltype(a_original)>(it);
  EXPECT_EQ(a_original, a_decoded);
  auto b_decoded = networking::SerializationUtils::DeserializeObject<decltype(b_original)>(it);
  EXPECT_EQ(b_original, b_decoded);
  auto c_decoded = networking::SerializationUtils::DeserializeObject<decltype(c_original)>(it);
  EXPECT_EQ(c_original, c_decoded);

  EXPECT_EQ(it, encoded.end());
}

TEST(GameSerialization, Projectile) {
  Player player("ABCD", Position(34, 56));
  Projectile original(10, 72, player.player_id, player.position, player.direction);

  auto deserialized = serialize_and_deserialize(original);

  EXPECT_EQ(original.GetDamage(), deserialized.GetDamage());
  EXPECT_EQ(original.GetDirection(), deserialized.GetDirection());
  EXPECT_EQ(original.GetShooterId(), deserialized.GetShooterId());
  EXPECT_EQ(original.GetRange(), deserialized.GetRange());

  EXPECT_EQ(original.GetPosition(), deserialized.GetPosition());
}

TEST(GameSerialization, Color) {
  Color original(12, 234, 56);

  auto deserialized = serialize_and_deserialize(original);

  EXPECT_EQ(original.red, deserialized.red);
  EXPECT_EQ(original.green, deserialized.green);
  EXPECT_EQ(original.blue, deserialized.blue);
}

TEST(GameSerialization, ColoredField) {
  Position position(0, 0);
  ColoredField original(position);

  auto deserialized = serialize_and_deserialize(original);

  EXPECT_EQ(original.GetLifetime(), deserialized.GetLifetime());
  EXPECT_EQ(original.GetPosition(), position);
}

TEST(GameSerialization, WorldWithColoredField) {
  World original(coordinate_size_t(20, 20));
  Position position(0, 0);
  ColoredField colored_field(position);
  original.AddColoredField(colored_field);
  EXPECT_TRUE(original.colored_fields.size() == 1);

  auto deserialized = serialize_and_deserialize(original);

  EXPECT_EQ(original.colored_fields.at(position).GetPosition(), deserialized.colored_fields.at(position).GetPosition());
  EXPECT_EQ(original.colored_fields.at(position).GetLifetime(), deserialized.colored_fields.at(position).GetLifetime());
}
