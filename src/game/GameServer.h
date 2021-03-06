#ifndef CROSSING_CONSOLES_GAMESERVER_H
#define CROSSING_CONSOLES_GAMESERVER_H

#include <memory>

#include "../communication/connection_layer/connection/ServerSideConnectionManager.h"
#include "networking/Change.h"
#include "world/World.h"
#include "world/items/ItemGenerator.h"

namespace game {

/**
 * \brief Server to handle game logic and connections to `GameClient`.
 */
class GameServer {
 public:
  explicit GameServer(
      const common::coordinate_size_t &world_size, bool empty_world = false,
      communication::ProtocolDefinition::timeout_t communication_timeout = communication::ProtocolDefinition::timeout,
      GameDefinition game_definition = GameDefinition());

  /**
   * \brief Handle connections to `GameClient`s. Should be called in a loop.
   * \param performance_mode Improves overall performance of the server. Increases time until a new client connects.
   */
  void RunIteration(bool performance_mode = false);

  [[nodiscard]] const world::World &GetWorld() const;

 private:
  /**
   * \brief Handle an `Event` caused by a `GameClient`.
   */
  void HandleEvent(const std::shared_ptr<communication::connection_layer::Event> &event);

  std::shared_ptr<world::World> world;

  std::shared_ptr<communication::connection_layer::ServerSideConnectionManager> server_manager;

  /// Used to send world updates at a constant frequency.
  std::chrono::time_point<std::chrono::steady_clock> last_world_sent;
  static constexpr auto send_world_interval = std::chrono::milliseconds(25);

  /// Handle moving projectiles at a constant frequency.
  std::chrono::time_point<std::chrono::steady_clock> last_moving_projectiles_updated;
  static constexpr auto update_projectiles_interval = std::chrono::milliseconds(50);

  /// Used to generate new items in the world
  std::chrono::time_point<std::chrono::steady_clock> last_item_generated;
  static constexpr auto generate_item_interval = std::chrono::seconds(2);

  /// Used to improve performance by accepting new clients less often
  std::chrono::time_point<std::chrono::steady_clock> last_full_connection_handle;
  static constexpr auto full_connection_handle_interval = std::chrono::milliseconds(100);

  static constexpr auto min_run_loop_interval = std::chrono::microseconds(25);

  static constexpr int max_player_count = 25;

  /// Holds all game definitions valid for this game.
  const GameDefinition game_definition;
};

}  // namespace game

#endif  // CROSSING_CONSOLES_GAMESERVER_H
