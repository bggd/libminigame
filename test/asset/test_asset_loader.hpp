#ifndef MINIGAME_TEST_ASSET_ASSET_MANAGER_HPP_INCLUDED
#define MINIGAME_TEST_ASSET_ASSET_MANAGER_HPP_INCLUDED

#include <boost/core/lightweight_test.hpp>

#include "../../src/asset/asset.hpp"
#include "../../src/asset/asset_loader.hpp"
#include "../../src/asset/decoder.hpp"

#include <variant>
#include <thread>
#include <chrono>
#include <stdint.h>


void test_asset_loader()
{
  AssetLoader<asset_t, asset_specific_t, DecodeVisitor<asset_t>> al;
  al.load("circle.tga", AssetImageTGA{});

  BOOST_TEST_EQ(al.is_complete(), false);
  BOOST_TEST_EQ(al.task_count, 1);

  std::thread th_load(&decltype(al)::thread_for_load_file, &al);
  std::thread th_decode(&decltype(al)::thread_for_decode_buffer, &al);

  while (!al.is_complete()) {
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }

  BOOST_TEST(al.is_complete());
  BOOST_TEST_EQ(al.task_count, 0);
  BOOST_TEST(al.get("circle.tga").has_value());

  al.queue_load.close();
  al.queue_decode.close();

  th_load.join();
  th_decode.join();

  delete[] std::get<AssetImage>(al.assets["circle.tga"]).data;
  al.assets.clear();
}

#endif // MINIGAME_TEST_ASSET_ASSET_MANAGER_HPP_INCLUDED
