#ifndef MINIGAME_SRC_ASSET_ASSET_LOADER_HPP_INCLUDED
#define MINIGAME_SRC_ASSET_ASSET_LOADER_HPP_INCLUDED

#include "../thread/thread_queue.hpp"
#include "asset.hpp"
#include "../third_party/debug_assert.hpp"

#include <variant>
#include <optional>
#include <map>
#include <string_view>
#include <memory>
#include <fstream>
#include <mutex>
#include <limits>
#include <stdint.h>
#include <string.h>
#include <assert.h>


struct AssetLoader {

  struct AssetFile {

    std::string_view path;
    uint8_t* data = nullptr;
    size_t length = 0;
    asset_t type;

  };

  ThreadQueue<AssetFile> queue_load;
  ThreadQueue<AssetFile> queue_decode;
  std::atomic<uint32_t> task_count = 0;
  std::mutex mtx;
  std::map<std::string_view, std::shared_ptr<AssetBase>> assets;

  void thread_for_load_file() noexcept;
  void thread_for_decode_buffer() noexcept;

  void load(std::string_view path, asset_t asset) noexcept;
  bool is_complete() const noexcept;
  std::optional<std::weak_ptr<AssetBase>> get(std::string_view key) noexcept;

};


void AssetLoader::thread_for_load_file() noexcept
{
  for (;;) {
    std::optional<AssetFile> opt = std::move(this->queue_load.pop());

    if (!opt) {
      DEBUG_ASSERT(this->queue_decode.is_close == true, assert_handler{});
      break;
    }

    AssetFile af = std::move(*opt);

    std::ifstream file(af.path.data(), std::ios_base::binary | std::ios_base::ate);

    DEBUG_ASSERT(file.is_open(), assert_handler{});

    auto file_len = file.tellg();
    DEBUG_ASSERT(file_len, assert_handler{});
    file.seekg(0, std::ios_base::beg);

    static_assert(sizeof(char) == sizeof(uint8_t));
    uint8_t* buf = new uint8_t[file_len];

    if (file.read((char*)buf, file_len)) {
      af.data = buf;
      af.length = file_len;

      this->queue_decode.emplace(std::move(af));
    }
    else {
      fprintf(stderr, "couldn't read file: %s\n", af.path.data());
      abort();
    }
  }
}

void AssetLoader::thread_for_decode_buffer() noexcept
{
  for (;;) {
    std::optional<AssetFile> opt = std::move(this->queue_decode.pop());

    if (!opt) {
      DEBUG_ASSERT(this->queue_decode.is_close == true, assert_handler{});
      break;
    }

    AssetFile af = std::move(*opt);

    AssetBase* ap = std::visit([](auto x) {
      try { return static_cast<AssetBase*>(new decltype(x)); }
      catch (const std::exception& e) { DEBUG_UNREACHABLE(assert_handler{}, e.what()); abort(); }
    }, af.type);

    DEBUG_ASSERT(ap, assert_handler{});

    ap->load_from_memory(af.data, af.length);

    try {
      this->assets[af.path] = std::shared_ptr<AssetBase>(
        ap,
        [] (AssetBase* p) {
          DEBUG_ASSERT(p, assert_handler{});
          p->unload();
          delete p;
        }
      );
    }
    catch (const std::exception& e) {
      DEBUG_UNREACHABLE(assert_handler{}, e.what());
      abort();
    }

    DEBUG_ASSERT(this->task_count > 0, assert_handler{});
    this->task_count--;
  }
}

void AssetLoader::load(std::string_view path, asset_t asset) noexcept
{
  AssetFile af;
  af.path = path;
  af.type = std::move(asset);
  
  DEBUG_ASSERT(this->task_count < std::numeric_limits<uint32_t>::max(), assert_handler{});
  this->task_count++;

  this->queue_load.emplace(std::move(af));
}

bool AssetLoader::is_complete() const noexcept
{
  return this->task_count == 0;
}

std::optional<std::weak_ptr<AssetBase>> AssetLoader::get(std::string_view key) noexcept
{
  try {
    std::lock_guard lk(this->mtx);

    auto it = this->assets.find(key);

    if (it == this->assets.end()) { return std::nullopt; }
    else { return it->second; }
  }
  catch (const std::exception& e) {
    DEBUG_UNREACHABLE(assert_handler{}, e.what());
    abort();
  }
}

#endif // MINIGAME_SRC_ASSET_ASSET_LOADER_HPP_INCLUDED
