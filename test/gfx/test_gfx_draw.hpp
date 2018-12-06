#ifndef MINIGAME_TEST_GFX_TEST_GFX_DRAW_HPP_INCLUDED
#define MINIGAME_TEST_GFX_TEST_GFX_DRAW_HPP_INCLUDED

#include <gtest/gtest.h>

#include "../../src/window/window.hpp"
#include "../../src/gfx/gfx_texture.hpp"
#include "../../src/gfx/gfx_draw.hpp"

TEST(GfxDraw, Test)
{
  uint8_t rgb[32*32*3];

  Window w;
  w.open();

  GfxTexture tex;
  tex.create_from_rgb(rgb, 32, 32);

  GfxDraw gfx;
  gfx.create(1024);

  gfx.draw_texture(&tex, 0, 0);
  gfx.draw();

  gfx.destroy();
  tex.destroy();

  w.close();

}

#endif // MINIGAME_TEST_GFX_TEST_GFX_DRAW_HPP_INCLUDED
