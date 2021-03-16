#include <gtest/gtest.h>

#include "../src/game/visual/Renderer.h"
#include "../src/game/visual/symbols.h"

using namespace game::visual;
using namespace game::common;
using namespace game::world;
using namespace game::visual::symbols;

TEST(Renderer, EmptyWorld) {
  coordinate_size_t world_size = coordinate_size_t(101, 101);
  coordinate_size_t viewport_size = coordinate_size_t(41, 21);
  coordinate_size_t block_size = coordinate_size_t(2, 2);
  Position player_position = Position(0, 0);
  Player player = Player("player", player_position);
  World world = World(world_size);
  Renderer renderer(viewport_size, block_size, world, player);

  std::wstring rendered_world = renderer.RenderWorld();

  ASSERT_TRUE(!rendered_world.empty());

  int n;
  for (int y = 0; y < viewport_size.y * block_size.y; y++) {
    for (int x = 0; x < viewport_size.x * block_size.x + 1; x++) {
      n = (viewport_size.x * block_size.x + 1) * y + x;
      if (x == viewport_size.x * block_size.x) {
        EXPECT_EQ(rendered_world[n], L'\n');
      } else {
        EXPECT_EQ(rendered_world[n], L' ');
      }
    }
  }
}