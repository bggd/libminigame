#ifndef MINIGAME_SRC_ASSET_ASSET_LOADER_HPP_INCLUDED
#define MINIGAME_SRC_ASSET_ASSET_LOADER_HPP_INCLUDED

#include "../thread/thread_queue.hpp"
#include "asset.hpp"

#include <variant>
#include <map>
#include <string_view>
#include <memory>
#include <fstream>
#include <mutex>
#include <stdint.h>
#include <string.h>
#include <assert.h>


template <typename AssetOutput, typename AssetVariant, typename AssetDecoder>
struct AssetLoader {

  struct AssetFile {

    std::string_view path;
    uint8_t* data = nullptr;
    size_t length = 0;
    AssetVariant asset_type;

  };

  AssetDecoder decoder;
  ThreadQueue<AssetFile> queue_load;
  ThreadQueue<AssetFile> queue_decode;
  std::atomic<uint32_t> task_count = 0;
  std::mutex mtx;
  std::map<std::string_view, std::shared_ptr<const AssetOutput>> assets;

  void thread_for_load_file() noexcept;
  void thread_for_decode_buffer() noexcept;

  void load(std::string_view path, asset_specific_t asset);
  bool is_complete() const noexcept;
  std::optional<std::weak_ptr<const AssetOutput>> get(std::string_view key) noexcept;

};

template <typename AssetOutput, typename AssetVariant, typename AssetDecoder>
void AssetLoader<AssetOutput, AssetVariant, AssetDecoder>::thread_for_load_file() noexcept
{
  try {
    for (;;) {
      std::optional<AssetFile> opt = std::move(this->queue_load.pop());
      if (!opt) { break; }
      AssetFile af = opt.value();
      std::ifstream file(af.path.data(), std::ios_base::binary | std::ios_base::ate);
      assert(file.is_open());
      std::string str;
      str.resize(file.tellg());
      file.seekg(0, std::ios_base::beg);
      if (file.read(&str[0], str.length())) {
        af.data = new uint8_t[str.length()];
        memcpy(af.data, str.data(), str.length());
        af.length = str.length();
        this->queue_decode.push(af);
      }
      else {
        fprintf(stderr, "couldn't read file: %s\n", af.path.data());
        abort();
      }
    }
  }
  catch (const std::exception& e) {
    fprintf(stderr, "%s\n", e.what());
    abort();
  }
}

template <typename AssetOutput, typename AssetVariant, typename AssetDecoder>
void AssetLoader<AssetOutput, AssetVariant, AssetDecoder>::thread_for_decode_buffer() noexcept
{
  try {
    for (;;) {
      std::optional<AssetFile> opt_af = std::move(this->queue_decode.pop());
      if (!opt_af) { break; }
      AssetFile af = opt_af.value();
      AssetLoadedFileData d;
      d.data = af.data;
      d.length = af.length;
      asset_specific_t v = d;
      std::optional<AssetOutput> opt = std::visit(decoder, af.asset_type, v);
      assert(opt.has_value());
      delete[] af.data;
      std::lock_guard<std::mutex> lk(this->mtx);
      this->assets[af.path] = std::shared_ptr<AssetOutput>(
        new AssetOutput(opt.value()),
        [](AssetOutput* p){
          std::visit([](auto x) { delete[] x.data; }, *p);
        }
      );
      this->task_count--;
    }
  }
  catch (const std::exception& e)
  {
    fprintf(stderr, "%s\n", e.what());
    abort();
  }
}

template <typename AssetOutput, typename AssetVariant, typename AssetDecoder>
void AssetLoader<AssetOutput, AssetVariant, AssetDecoder>::load(std::string_view path, asset_specific_t asset)
{
  AssetFile af;
  af.path = path;
  af.asset_type = asset;
  this->task_count++;
  this->queue_load.push(af);
}

template <typename AssetOutput, typename AssetVariant, typename AssetDecoder>
bool AssetLoader<AssetOutput, AssetVariant, AssetDecoder>::is_complete() const noexcept
{
  return this->task_count == 0;
}

template <typename AssetOutput, typename AssetVariant, typename AssetDecoder>
std::optional<std::weak_ptr<const AssetOutput>> AssetLoader<AssetOutput, AssetVariant, AssetDecoder>::get(std::string_view key) noexcept
{
  try {
    std::lock_guard<std::mutex> lk(this->mtx);
    auto it = this->assets.find(key);
    if (it == this->assets.end()) { return std::nullopt; }
    else { return std::weak_ptr<const AssetOutput>(it->second); }
  }
  catch (const std::exception& e) {
    fprintf(stderr, "%s\n", e.what());
    abort();
  }
}

#endif // MINIGAME_SRC_ASSET_ASSET_LOADER_HPP_INCLUDED
