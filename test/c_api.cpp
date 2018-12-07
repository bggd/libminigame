#include <gtest/gtest.h>

#include "../include/minigame/minigame.h"


TEST(CAPI, Test)
{
  minigame_init();

  minigame_window_open();
  minigame_window_swap_buffers();
  minigame_window_close();

  minigame_deinit();
}

int main(int argc, char** argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
