#ifndef MINIGAME_SRC_ASSET_DECODER_TGA_HPP_INCLUDED
#define MINIGAME_SRC_ASSET_DECODER_TGA_HPP_INCLUDED

#include "asset.hpp"
#include "../third_party/stb_image.hpp"


template <typename AssetOutput>
struct DecoderTGA {

  std::optional<AssetOutput> operator()(AssetImageTGA&, AssetLoadedFileData& file_data)
  {
    int x, y, n;
    uint8_t* data = stbi_load_from_memory(
      file_data.data,
      static_cast<int>(file_data.length),
      &x, &y, &n, 0
    );
    if (data == nullptr) { return std::nullopt; }

    AssetImage img;
    if (n == 1) { img.format = AssetImage::PixelFormat::GREY; }
    else if (n == 2) { img.format = AssetImage::PixelFormat::GREY_ALPHA; }
    else if (n == 3) { img.format = AssetImage::PixelFormat::RGB; }
    else if (n == 4) { img.format = AssetImage::PixelFormat::RGBA; }
    img.width = static_cast<uint16_t>(x);
    img.height = static_cast<uint16_t>(y);
    img.data = data;

    return img;
  }

};

#endif // MINIGAME_SRC_ASSET_DECODER_TGA_HPP_INCLUDED
