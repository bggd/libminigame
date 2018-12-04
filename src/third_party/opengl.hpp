#ifndef MINIGAME_SRC_THIRD_PARTY_OPENGL_HPP_INCLUDED
#define MINIGAME_SRC_THIRD_PARTY_OPENGL_HPP_INCLUDED

#include "glfw.hpp"

#include <stdint.h>

using GLvoid = void;
using GLbyte  = char;
using GLubyte  = unsigned char;
using GLshort = int16_t;
using GLuhort = uint16_t;
using GLint = int32_t;
using GLuint = uint32_t;
using GLenum = uint32_t;
using GLintptr = intptr_t;
using GLsizeiptr = ptrdiff_t;
using GLbitfield = uint32_t;
using GLfloat = float;

#define GL_FALSE 0
#define GL_TRUE 1

#define GL_COLOR_BUFFER_BIT 0x00000100

#define GL_VENDOR 0x1f00
#define GL_RENDERER 0x1f01
#define GL_VERSION 0x1f02
#define GL_SHADING_LANGUAGE_VERSION 0x8b8c

#define GL_DEF(ret, name, ...) ret (*gl##name)(__VA_ARGS__) = NULL;

#define GL_FUNCS \
  GL_DEF(GLubyte*, GetString, GLenum) \
  GL_DEF(void, Clear, GLbitfield)

GL_FUNCS

bool load_gl()
{
  #undef GL_DEF
  #define GL_DEF(ret, name, ...) \
  gl##name = (decltype(gl##name))glfwGetProcAddress("gl"#name); \
  if (gl##name == NULL) { return false; }

  GL_FUNCS

  return true;
}

#endif // MINIGAME_SRC_THIRD_PARTY_OPENGL_HPP_INCLUDED
