#ifndef MINIGAME_TEST_ASSET_ASSET_LOADER_HPP_INCLUDED
#define MINIGAME_TEST_ASSET_ASSET_LOADER_HPP_INCLUDED

#include <gtest/gtest.h>

#include "../../src/asset/asset.hpp"
#include "../../src/asset/asset_loader.hpp"

#include <variant>
#include <thread>
#include <chrono>
#include <type_traits>
#include <stdint.h>

static_assert(std::is_nothrow_move_constructible<std::optional<AssetLoader::AssetFile>>::value);
static_assert(std::is_nothrow_move_assignable<std::optional<AssetLoader::AssetFile>>::value);
static_assert(std::is_nothrow_move_constructible<std::optional<asset_t>>::value);
static_assert(std::is_nothrow_move_assignable<std::optional<asset_t>>::value);

TEST(AssetLoader, Test)
{
  AssetLoader al;
  al.load("circle.tga", AssetImageTGA{});
  al.load("Upbeat Loop.ogg", AssetAudioVorbis{});

  ASSERT_FALSE(al.is_complete());
  ASSERT_TRUE(al.task_count == 2);

  std::thread th_load(&decltype(al)::thread_for_load_file, &al);
  std::thread th_decode(&decltype(al)::thread_for_decode_buffer, &al);

  while (!al.is_complete()) {
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }

  ASSERT_TRUE(al.is_complete());
  ASSERT_TRUE(al.task_count == 0);

  ASSERT_TRUE(al.get("circle.tga").has_value());
  ASSERT_TRUE(al.get("Upbeat Loop.ogg").has_value());

  al.queue_load.close();
  al.queue_decode.close();

  th_load.join();
  th_decode.join();
}

#endif // MINIGAME_TEST_ASSET_ASSET_LOADER_HPP_INCLUDED
