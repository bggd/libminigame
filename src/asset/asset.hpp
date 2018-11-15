#ifndef MINIGAME_SRC_ASSET_ASSET_HPP_INCLUDED
#define MINIGAME_SRC_ASSET_ASSET_HPP_INCLUDED

#include "../third_party/debug_assert.hpp"

#include <variant>
#include <stdint.h>


struct AssetBase {

  virtual ~AssetBase() {}
  virtual void load_from_memory(uint8_t*, size_t) noexcept = 0;
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

  virtual void load_from_memory(uint8_t*, size_t) noexcept override { DEBUG_UNREACHABLE(assert_handler{}); }
  virtual void unload() noexcept override { DEBUG_UNREACHABLE(assert_handler{}); }

};

struct AssetAudio : AssetBase {

  enum class Format {
    STEREO_SHORT16,
    MONO_SHORT16
  };

  Format format;
  uint32_t  length;
  int16_t* data;

  virtual void load_from_memory(uint8_t*, size_t) noexcept override { DEBUG_UNREACHABLE(assert_handler{}); }
  virtual void unload() noexcept override { /*DEBUG_UNREACHABLE(assert_handler{});*/ }

};

#include "asset_tga.hpp"
#include "asset_vorbis.hpp"

using asset_t = std::variant<AssetImageTGA, AssetAudioVorbis>;
using asset_audio_t = std::variant<AssetAudioVorbis>;

#endif // MINIGAME_SRC_ASSET_ASSET_HPP_INCLUDED
