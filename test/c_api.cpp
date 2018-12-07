#include <gtest/gtest.h>

#include "../include/minigame/minigame.h"

TEST(CAPI, Test)
{
  minigame_init();
  minigame_deinit();
}

int main(int argc, char** argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
