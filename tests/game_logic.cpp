#include <gtest/gtest.h>

#include "../src/game/GameClient.h"
#include "../src/game/GameLogic.h"
#include "fixtures/GamePlay.h"

using namespace game;
using namespace game::common;
using namespace game::world;

TEST_F(GamePlay, ProjectilesNoHit) {
  // Projectile flies through the world without a hit

  uint8_t range = 3;  // any range >0 is possible

  initialize_game();
  spawn_projectile(range);
  ASSERT_EQ(world->GetProjectiles().size(), 1);

  for (int i = 0; i < range; i++) {
    GameLogic::HandleProjectiles(*world);
    ASSERT_EQ(world->GetProjectiles().size(), 1);
  }

  ASSERT_FALSE(world->GetProjectiles().begin()->get()->StillFlying());
  GameLogic::HandleProjectiles(*world);
  ASSERT_TRUE(world->GetProjectiles().empty());
  reset_elements();
}

TEST_F(GamePlay, ProjectileHitsPlayer) {
  // Projectile hits player and decreases their health

  uint8_t range = 10;  // range must be at least 1 in order to hit player
  int damage = 34;     // any damage is possible

  initialize_game();
  add_player();

  player_first->direction = GameDefinition::Direction::WEST;
  player_second->position.x = player_first->position.x;
  player_second->position.y = player_first->position.y;

  spawn_projectile(range, damage, player_first->direction, player_first->player_id, player_first->position);
  move_player(player_second, game::networking::ChangeType::MOVE_LEFT, 2);

  // Move projectile
  for (int i = 0; i < range; i++) {
    GameLogic::HandleProjectiles(*world);
    if (world->GetProjectiles().empty()) {
      break;
    }
  }

  ASSERT_TRUE(player_second->health < player_second->max_health);
  ASSERT_TRUE(player_first->GetScore() > 0);
  ASSERT_TRUE(world->GetProjectiles().empty());
  reset_elements();
}

TEST_F(GamePlay, ProjectileHitsWall) {
  // Projectile hits a wall

  uint8_t range = 10;  // range must be at least 1 in order to hit player

  initialize_game();
  spawn_projectile(range);
  world->AddWall(Position(1, 0));

  GameLogic::HandleProjectiles(*world);

  ASSERT_TRUE(world->GetProjectiles().empty());
  reset_elements();
}

TEST_F(GamePlay, ProjectileHitsProjectile) {
  // Projectile hits another projectile

  initialize_game();

  std::list<GameDefinition::Direction> direction_list = {GameDefinition::WEST, GameDefinition::SOUTH,
                                                         GameDefinition::EAST, GameDefinition::NORTH};

  for (auto& direction_first : direction_list) {
    for (auto& direction_second : direction_list) {
      if (direction_second != direction_first) {
        projectile_collision(direction_first, direction_second);
      }
    }
  }

  reset_elements();
}

TEST_F(GamePlay, MultipleProjectileSameShooter) {
  // Projectile at the same position with common shooter_id & direction are merged to one projectile

  initialize_game();

  uint8_t range = 3;  // needs to be at least 1
  int damage = 3;     // arbitrarily chosen
  int number_of_bullets = 10;

  for (int i = 0; i < number_of_bullets; i++) {
    spawn_projectile(range, damage);
  }

  GameLogic::HandleProjectiles(*world);

  EXPECT_FALSE(world->GetProjectiles().empty());
  ASSERT_EQ(world->GetProjectiles().size(), 1);

  reset_elements();
}

TEST_F(GamePlay, PlayerMovesIntoProjectile) {
  // Player moves into projectile

  initialize_game();
  add_player();

  // range and damage chosen arbitrarily, range >=1
  spawn_projectile(4, 20, GameDefinition::NORTH);

  GameLogic::HandleProjectiles(*world);

  // Set player_second next to bullet and move into it
  player_second->position.Set(-1, -1);
  move_player(player_second, game::networking::ChangeType::MOVE_RIGHT, 1);

  // bullet should be deleted & player_second hurt
  ASSERT_TRUE(world->GetProjectiles().empty());
  ASSERT_TRUE(player_second->health < player_second->max_health);

  reset_elements();
}

TEST_F(GamePlay, PlayerHitByOwnProjectile) {
  // Projectile hits its shooter and decreases their health but not their score

  uint8_t range = 10;  // range must be at least 1 in order to hit player
  int damage = 2;      // any damage is possible

  initialize_game();
  add_player();

  uint8_t player_score = 5;
  player_first->IncreaseScore(5);

  player_first->direction = GameDefinition::Direction::WEST;
  spawn_projectile(range, damage, player_first->direction, player_first->player_id, player_first->position);

  move_player(player_first, game::networking::ChangeType::MOVE_LEFT, 2);

  // Move projectile
  for (int i = 0; i < range; i++) {
    GameLogic::HandleProjectiles(*world);
    if (world->GetProjectiles().empty()) {
      break;
    }
  }

  ASSERT_TRUE(player_first->health < player_first->max_health);
  ASSERT_EQ(player_first->GetScore(), player_score);
  ASSERT_TRUE(world->GetProjectiles().empty());
  reset_elements();
}

TEST_F(GamePlay, ProjectileHitsDeadPlayer) {
  // Projectile hits dead player, no consequences occur

  uint8_t range = 10;  // range must be at least 1 in order to reach player
  int damage = 34;     // any damage is possible

  initialize_game();
  add_player();

  auto initial_score_player_first = player_first->GetScore();

  player_first->direction = GameDefinition::Direction::WEST;
  player_second->position.x = player_first->position.x;
  player_second->position.y = player_first->position.y;

  move_player(player_second, game::networking::ChangeType::MOVE_LEFT, 2);

  // Kill player_second
  player_second->DecreaseHealth(player_second->max_health);
  EXPECT_FALSE(player_second->IsAlive());

  spawn_projectile(range, damage, player_first->direction, player_first->player_id, player_first->position);

  // Move projectile
  for (int i = 0; i < range; i++) {
    GameLogic::HandleProjectiles(*world);
    ASSERT_FALSE(world->GetProjectiles().empty());
  }

  ASSERT_EQ(player_first->GetScore(), initial_score_player_first);
  reset_elements();
}

TEST_F(GamePlay, DeadPlayerNoChange) {
  // Dead player invokes changes, nothing happens

  initialize_game();
  add_player();

  // Kill player_first
  player_first->DecreaseHealth(player_first->max_health);
  EXPECT_FALSE(player_first->IsAlive());

  auto initial_position = player_first->position;

  // try to move in any direction
  move_player(player_first, game::networking::ChangeType::MOVE_LEFT, 2);
  move_player(player_first, game::networking::ChangeType::MOVE_RIGHT, 1);
  move_player(player_first, game::networking::ChangeType::MOVE_DOWN, 7);
  move_player(player_first, game::networking::ChangeType::MOVE_UP, 10);

  ASSERT_EQ(player_first->position, initial_position);

  game::networking::Change item_change(game::networking::ChangeType::USE_ITEM);
  GameLogic::HandleChange(*player_first, item_change, *world);

  ASSERT_TRUE(world->GetProjectiles().empty());

  reset_elements();
}