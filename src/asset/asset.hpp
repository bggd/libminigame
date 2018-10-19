#ifndef MINIGAME_SRC_ASSET_ASSET_HPP_INCLUDED
#define MINIGAME_SRC_ASSET_ASSET_HPP_INCLUDED

#include <variant>
#include <stdint.h>


struct AssetImage {

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


};

struct AssetAudio {

  enum class Format {
    STEREO_SHORT16,
    MONO_SHORT16
  };

  Format format;
  uint32_t  length;
  int16_t* data;

};

struct AssetImageTGA {};
struct AssetAudioVorbis {};

struct AssetLoadedFileData {

  const uint8_t* data;
  size_t length;

};

using asset_t = std::variant<AssetImage, AssetAudio>;
using asset_specific_t = std::variant<AssetLoadedFileData, AssetImageTGA, AssetAudioVorbis>;

#endif // MINIGAME_SRC_ASSET_ASSET_HPP_INCLUDED
