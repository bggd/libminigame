#ifndef MINIGAME_SRC_GFX_GFX_SHADER_HPP_INCLUDED
#define MINIGAME_SRC_GFX_GFX_SHADER_HPP_INCLUDED

#include "../third_party/opengl.hpp"
#include "../third_party/debug_assert.hpp"


struct GfxShader {

  GLuint id = 0;

  void create_from_memory(const char* vertex_glsl, const char* fragment_glsl) noexcept
  {
    this->id = glCreateProgram();
    DEBUG_ASSERT(this->id, assert_handler{});

    GLuint vs, fs;

    GL_CHECK(vs = glCreateShader(GL_VERTEX_SHADER));
    DEBUG_ASSERT(vs, assert_handler{});
    GL_CHECK(glShaderSource(vs, 1, &vertex_glsl, nullptr));
    GL_CHECK(glCompileShader(vs));

    GLint status;
    GL_CHECK(glGetShaderiv(vs, GL_COMPILE_STATUS, &status));
    DEBUG_ASSERT(status == GL_TRUE, assert_handler{});

    GL_CHECK(fs = glCreateShader(GL_FRAGMENT_SHADER));
    DEBUG_ASSERT(fs, assert_handler{});
    GL_CHECK(glShaderSource(fs, 1, &fragment_glsl, nullptr));
    GL_CHECK(glCompileShader(fs));

    GL_CHECK(glGetShaderiv(fs, GL_COMPILE_STATUS, &status));
    DEBUG_ASSERT(status == GL_TRUE, assert_handler{});

    GL_CHECK(glAttachShader(this->id, vs));
    GL_CHECK(glAttachShader(this->id, fs));
    GL_CHECK(glLinkProgram(this->id));

    GL_CHECK(glGetProgramiv(this->id, GL_LINK_STATUS, &status));
    DEBUG_ASSERT(status == GL_TRUE, assert_handler{});

    GL_CHECK(glDetachShader(this->id, vs));
    GL_CHECK(glDetachShader(this->id, fs));
  }

  void destroy() noexcept
  {
    GL_CHECK(glDeleteProgram(this->id));
  }

  void use() noexcept
  {
    GL_CHECK(glUseProgram(this->id));
  }

};

#endif // MINIGAME_SRC_GFX_GFX_SHADER_HPP_INCLUDED
