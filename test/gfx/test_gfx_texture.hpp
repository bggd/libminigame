#ifndef MINIGAME_TEST_GFX_TEST_GFX_TEXTURE_HPP_INCLUDED
#define MINIGAME_TEST_GFX_TEST_GFX_TEXTURE_HPP_INCLUDED

#include <gtest/gtest.h>

#include "../../src/window/window.hpp"
#include "../../src/gfx/gfx_texture.hpp"
#include <stdint.h>


TEST(GfxTexture, Test)
{
  uint8_t rgb[32*32];

  Window w;
  w.open();

  GfxTexture tex;
  tex.create_from_rgb(rgb, 32, 32);
  tex.destroy();

  w.close();
}

#endif // MINIGAME_TEST_GFX_TEST_GFX_TEXTURE_HPP_INCLUDED
