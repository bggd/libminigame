#include <gtest/gtest.h>

#include "thread/test_thread_queue.hpp"
#include "asset/test_asset_loader.hpp"
#include "audio/test_audio_player.hpp"
#include "window/test_window.hpp"


int main(int argc, char** argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
