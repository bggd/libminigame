#ifndef MINIGAME_SRC_GFX_GFX_TEXTURE_HPP_INCLUDED
#define MINIGAME_SRC_GFX_GFX_TEXTURE_HPP_INCLUDED

#include "../third_party/opengl.hpp"
#include "../third_party/debug_assert.hpp"


struct GfxTexture {

  GLuint id;
  uint16_t w;
  uint16_t h;

  void create_from_rgb(uint8_t* data, uint16_t width, uint16_t height) noexcept;
  void destroy() noexcept;
  void bind() noexcept;

};

void GfxTexture::create_from_rgb(uint8_t* data, uint16_t width, uint16_t height) noexcept
{
  GL_CHECK(glGenTextures(1, &this->id));
  GL_CHECK(glBindTexture(GL_TEXTURE_2D, this->id));
  DEBUG_ASSERT(glIsTexture(this->id) == GL_TRUE, assert_handler{});
  GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
  GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
  GL_CHECK(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data));
  this->w = width;
  this->h = height;
}

void GfxTexture::destroy() noexcept
{
  GL_CHECK(glDeleteTextures(1, &this->id));
}

void GfxTexture::bind() noexcept
{
  GL_CHECK(glBindTexture(GL_TEXTURE_2D, this->id));
}


#endif // MINIGAME_SRC_GFX_GFX_TEXTURE_HPP_INCLUDED
