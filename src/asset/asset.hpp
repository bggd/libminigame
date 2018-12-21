#ifndef MINIGAME_SRC_ASSET_ASSET_HPP_INCLUDED
#define MINIGAME_SRC_ASSET_ASSET_HPP_INCLUDED

#include "../third_party/debug_assert.hpp"
#include "../gfx/gfx_texture.hpp"

#include <variant>
#include <array>
#include <stdint.h>


struct AssetBase {

  virtual ~AssetBase() {}
  virtual void load_from_memory(uint8_t* file_data, size_t file_length) noexcept = 0;
  virtual void unload() noexcept = 0;

};

struct AssetImage : AssetBase {

  enum class PixelFormat {
    GREY,
    GREY_ALPHA,
    RGB,
    RGBA
  };

  PixelFormat  format;
  uint16_t width;
  uint16_t height;
  uint8_t* data;
  GfxTexture texture;
  bool is_uploaded = false;

  virtual void load_from_memory(uint8_t*, size_t) noexcept = 0;
  virtual void unload() noexcept = 0;
  void create_texture() noexcept;
  void destroy_texture() noexcept;

};

struct AssetAudio : AssetBase {

  enum class Format {
    MONO_SHORT16,
    STEREO_SHORT16
  };

  Format format;
  bool is_static = true;
  int16_t* raw_samples;
  int32_t num_samples;

  virtual void load_from_memory(uint8_t*, size_t) noexcept = 0;
  virtual void unload() noexcept = 0;
  virtual size_t decode(int16_t* data, size_t num_elem) noexcept = 0;
  virtual void rewind() noexcept = 0;

};

#include "asset_file.hpp"
#include "asset_tga.hpp"
#include "asset_vorbis.hpp"

using asset_t = std::variant<AssetFile, AssetImageTGA, AssetAudioVorbis>;

#include "../third_party/debug_assert.hpp"

#include <string_view>
#include <fstream>
#include <variant>


void asset_load_file(const std::string_view path, uint8_t** ret_buf, size_t* ret_len) noexcept
{
  std::ifstream file(path.data(), std::ios_base::binary | std::ios_base::ate);

  DEBUG_ASSERT(file.is_open(), assert_handler{});
  auto file_len = file.tellg();
  DEBUG_ASSERT(file_len, assert_handler{});
  file.seekg(0, std::ios_base::beg);

  static_assert(sizeof(char) == sizeof(uint8_t));
  uint8_t* buf = new uint8_t[file_len];

  if (file.read((char*)buf, file_len)) {
    *ret_buf = buf;
    *ret_len = file_len;
  }
  else {
    fprintf(stderr, "couldn't read file: %s\n", path.data());
    abort();
  }
}

std::shared_ptr<AssetBase> asset_create(asset_t type, uint8_t* data, size_t length)
{
  AssetBase* ap = std::visit([](auto x) {
    try { return static_cast<AssetBase*>(new decltype(x)); }
    catch (const std::exception& e) { DEBUG_UNREACHABLE(assert_handler{}, e.what()); abort(); }
  }, type);

  DEBUG_ASSERT(ap, assert_handler{});

  ap->load_from_memory(data, length);

  try {
    return std::shared_ptr<AssetBase>(
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
}

void AssetImage::create_texture() noexcept
{
  if (this->format == AssetImage::PixelFormat::RGB) {
    this->texture.create_from_rgb(this->data, this->width, this->height);
  }
  else if (this->format == AssetImage::PixelFormat::RGBA) {
    this->texture.create_from_rgba(this->data, this->width, this->height);
  }
  else {
    DEBUG_UNREACHABLE(assert_handler{});
  }
  this->is_uploaded = true;
}

void AssetImage::destroy_texture() noexcept
{
  if (this->is_uploaded) { this->texture.destroy(); }
}

#endif // MINIGAME_SRC_ASSET_ASSET_HPP_INCLUDED
