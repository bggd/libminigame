#include <gtest/gtest.h>

#include "thread/test_thread_queue.hpp"
#include "asset/test_asset_loader.hpp"


int main(int argc, char** argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
