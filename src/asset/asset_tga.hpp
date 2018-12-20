#ifndef MINIGAME_SRC_ASSET_ASSET_TGA_HPP_INCLUDED
#define MINIGAME_SRC_ASSET_ASSET_TGA_HPP_INCLUDED

#include "../third_party/stb_image.hpp"
#include "../third_party/debug_assert.hpp"

#include <limits>


struct AssetImageTGA : AssetImage {

  void load_from_memory(uint8_t* file_data, size_t file_length) noexcept override
  {
    int x, y, n;

    DEBUG_ASSERT(file_length <= std::numeric_limits<int>::max(), assert_handler{});

    this->data = stbi_load_from_memory(
      file_data,
      static_cast<int>(file_length),
      &x, &y, &n, 0
    );

    DEBUG_ASSERT(this->data, assert_handler{}, stbi_failure_reason());
    DEBUG_ASSERT(n > 0 && n < 5, assert_handler{});
    DEBUG_ASSERT(x <= std::numeric_limits<uint16_t>::max(), assert_handler{});
    DEBUG_ASSERT(y <= std::numeric_limits<uint16_t>::max(), assert_handler{});

    if (n == 1) { this->format = AssetImage::PixelFormat::GREY; }
    else if (n == 2) { this->format = AssetImage::PixelFormat::GREY_ALPHA; }
    else if (n == 3) { this->format = AssetImage::PixelFormat::RGB; }
    else if (n == 4) { this->format = AssetImage::PixelFormat::RGBA; }

    this->width = static_cast<uint16_t>(x);
    this->height = static_cast<uint16_t>(y);

    delete[] file_data;
  }

  void unload() noexcept override
  {
    stbi_image_free(this->data);
    this->destroy_texture();
  }

};

#endif // MINIGAME_SRC_ASSET_ASSET_TGA_HPP_INCLUDED
