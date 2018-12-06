#ifndef MINIGAME_SRC_GFX_GFX_TEXTURE_HPP_INCLUDED
#define MINIGAME_SRC_GFX_GFX_TEXTURE_HPP_INCLUDED

#include "../third_party/opengl.hpp"
#include "../third_party/debug_assert.hpp"


struct GfxTexture {

  GLuint id;

  void create_from_rgb(uint8_t* data, uint16_t width, uint16_t height)
  {
    GL_CHECK(glGenTextures(1, &this->id));
    GL_CHECK(glBindTexture(GL_TEXTURE_2D, this->id));
    DEBUG_ASSERT(glIsTexture(this->id) == GL_TRUE, assert_handler{});
    GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
    GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
    GL_CHECK(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data));
  }

  void destroy()
  {
    GL_CHECK(glDeleteTextures(1, &this->id));
  }

};

#endif // MINIGAME_SRC_GFX_GFX_TEXTURE_HPP_INCLUDED
