#ifndef CROSSING_CONSOLES_PLAYER_H
#define CROSSING_CONSOLES_PLAYER_H

#include <chrono>
#include <string>

#include "../GameDefinition.h"
#include "../common/Color.h"
#include "../common/Position.h"
#include "items/Gun.h"

namespace game::world {

/**
 * \brief Representation of a player.
 * \details Representation of a player with name, position, direction, health and information about his connection.
 */
class Player : public networking::ISerializable {
 public:
  std::string name;
  common::Color color;
  common::Position position;
  GameDefinition::player_id_t player_id;
  GameDefinition::Direction direction;

  static constexpr int max_health = 8;
  int health = max_health;
  bool updated = false;
  double packet_loss_percentage = 0;
  std::optional<std::chrono::microseconds> ping{};
  std::chrono::steady_clock::time_point time_of_death;
  uint16_t score = 0;

  Player(std::string name, common::Position position);
  Player(std::string name, common::Color color, common::Position position, GameDefinition::Direction direction,
         int player_id);

  [[nodiscard]] bool IsAlive() const;

  /**
   * \brief Set player's time of Death
   * \details Time of death is used to determine respawn.
   */
  void Die();

  void MoveTo(const common::Position& new_position);

  /**
   * \brief Decrease `Player`'s health by given damage.
   */
  void DecreaseHealth(uint8_t damage);

  /**
   * \brief Increase `Player`'s health by given healing.
   */
  void IncreaseHealth(uint8_t healing);

  void Serialize(std::vector<uint8_t>& output_vector) const override;

  static Player Deserialize(std::vector<uint8_t>::iterator& input_iterator);

  [[nodiscard]] uint16_t GetScore() const;

  /**
   * \brief Increase player's score by given `points`.
   * \details Standard increase is set to 1 point.
   */
  void IncreaseScore(uint16_t points = 1);

  /**
   * \brief Returns the player's item.
   */
  std::shared_ptr<IItem> GetItem();

  /**
   * \brief Assign new item to the `Player`if he walked into it. `Heart`s and `Point`s are used immediately.
   */
  void SetItem(std::shared_ptr<IItem> new_item);

  /**
   * \brief Remove the current item.
   */
  void RemoveItem();

 private:
  std::shared_ptr<IItem> item;
};

}  // namespace game::world

#endif  // CROSSING_CONSOLES_PLAYER_H
