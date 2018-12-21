#ifndef MINIGAME_SRC_THIRD_PARTY_OPENGL_HPP_INCLUDED
#define MINIGAME_SRC_THIRD_PARTY_OPENGL_HPP_INCLUDED

#include "../third_party/glfw.hpp"
#include "../third_party/debug_assert.hpp"

#include <string>
#include <stdint.h>

using GLboolean = unsigned char;
using GLvoid = void;
using GLbyte  = char;
using GLchar  = char;
using GLubyte  = unsigned char;
using GLshort = int16_t;
using GLuhort = uint16_t;
using GLint = int32_t;
using GLuint = uint32_t;
using GLenum = uint32_t;
using GLsizei = uint32_t;
using GLintptr = intptr_t;
using GLsizeiptr = ptrdiff_t;
using GLbitfield = uint32_t;
using GLfloat = float;

#define GL_FALSE 0
#define GL_TRUE 1

#define GL_COLOR_BUFFER_BIT 0x00004000

#define GL_VENDOR 0x1f00
#define GL_RENDERER 0x1f01
#define GL_VERSION 0x1f02
#define GL_SHADING_LANGUAGE_VERSION 0x8b8c

#define GL_TEXTURE_2D 0x0de1
#define GL_TEXTURE_MAG_FILTER 0x2800
#define GL_TEXTURE_MIN_FILTER 0x2801
#define GL_NEAREST 0x2600

#define GL_RGB 0x1907
#define GL_RGBA 0x1908
#define GL_RGB8 0x8051
#define GL_RGBA8 0x8058

#define GL_UNSIGNED_BYTE 0x1401
#define GL_FLOAT 0x1406

#define GL_NO_ERROR 0
#define GL_INVALID_ENUM 0x0500
#define GL_INVALID_VALUE 0x0501
#define GL_INVALID_OPERATION 0x0502
#define GL_STACK_OVERFLOW 0x0503
#define GL_STACK_UNDERFLOW 0x0504
#define GL_OUT_OF_MEMORY 0x0505
#define GL_INVALID_FRAMEBUFFER_OPERATION 0x0506
#define GL_CONTEXT_LOST 0x0507

#define GL_ARRAY_BUFFER 0x8892
#define GL_STREAM_DRAW 0x88e0
#define GL_BUFFER_SIZE 0x8764

#define GL_TRIANGLES 0x0004

#define GL_COMPILE_STATUS 0x8b81
#define GL_LINK_STATUS 0x8b82

#define GL_FRAGMENT_SHADER 0x8b30
#define GL_VERTEX_SHADER 0x8b31

#define GL_DEF(ret, name, ...) ret (*gl##name)(__VA_ARGS__) = NULL;

#define GL_FUNCS \
  GL_DEF(GLenum, GetError, GLvoid) \
  GL_DEF(GLubyte*, GetString, GLenum) \
  GL_DEF(void, Clear, GLbitfield) \
  GL_DEF(void, GenTextures, GLsizei, GLuint*) \
  GL_DEF(void, BindTexture, GLenum, GLuint) \
  GL_DEF(GLboolean, IsTexture, GLuint) \
  GL_DEF(void, TexParameteri, GLenum, GLenum, GLint) \
  GL_DEF(void, TexImage2D, GLenum, GLint, GLint, GLsizei, GLsizei, GLint, GLenum, GLenum, const GLvoid*) \
  GL_DEF(void, DeleteTextures, GLsizei, const GLuint*) \
  GL_DEF(void, GenBuffers, GLenum, GLuint*) \
  GL_DEF(void, BindBuffer, GLenum, GLuint) \
  GL_DEF(GLboolean, IsBuffer, GLuint) \
  GL_DEF(void, BufferData, GLenum, GLsizeiptr size, const GLvoid*, GLenum) \
  GL_DEF(void, GetBufferParameteriv, GLenum, GLenum, GLint*) \
  GL_DEF(void, DeleteBuffers, GLsizei, const GLuint*) \
  GL_DEF(void, VertexAttribPointer, GLuint, GLint, GLenum, GLboolean, GLsizei, const GLvoid*) \
  GL_DEF(void, GenVertexArrays, GLsizei, GLuint*) \
  GL_DEF(void, BindVertexArray, GLuint) \
  GL_DEF(GLboolean, IsVertexArray, GLuint) \
  GL_DEF(void, EnableVertexAttribArray, GLuint) \
  GL_DEF(void, DeleteVertexArrays, GLsizei, const GLuint*) \
  GL_DEF(void, DrawArrays, GLenum, GLint, GLsizei) \
  GL_DEF(GLuint, CreateProgram, GLvoid) \
  GL_DEF(GLuint, CreateShader, GLenum) \
  GL_DEF(void, ShaderSource, GLuint, GLsizei, const GLchar**, const GLint*) \
  GL_DEF(void, CompileShader, GLuint) \
  GL_DEF(void, GetShaderiv, GLuint, GLenum, GLint*) \
  GL_DEF(void, AttachShader, GLuint, GLuint) \
  GL_DEF(void, LinkProgram, GLuint) \
  GL_DEF(void, GetProgramiv, GLuint, GLenum, GLint*) \
  GL_DEF(void, DetachShader, GLuint, GLuint) \
  GL_DEF(void, DeleteProgram, GLuint) \
  GL_DEF(void, ClearColor, GLfloat, GLfloat, GLfloat, GLfloat) \
  GL_DEF(void, UseProgram, GLuint) \
  GL_DEF(void, Viewport, GLint, GLint, GLsizei, GLsizei) \
  GL_DEF(void, UniformMatrix4fv, GLint, GLsizei, GLboolean, const GLfloat*)

GL_FUNCS


bool load_gl() noexcept
{
  #undef GL_DEF
  #define GL_DEF(ret, name, ...) \
  gl##name = (decltype(gl##name))glfwGetProcAddress("gl"#name); \
  if (gl##name == NULL) { return false; }

  GL_FUNCS

  return true;
}

#define GL_CHECK(stmt) \
  do { \
    stmt; \
    opengl_check_error(#stmt, __FILE__, __LINE__); \
  } while (0);

const char* opengl_err2str(GLenum err) noexcept
{
  switch (err) {
    case GL_NO_ERROR: return "GL_NO_ERROR";
    case GL_INVALID_ENUM: return "GL_INVALID_ENUM";
    case GL_INVALID_VALUE: return "GL_INVALID_VALUE";
    case GL_INVALID_OPERATION: return "GL_INVALID_OPERATION";
    case GL_STACK_OVERFLOW: return "GL_STACK_OVERFLOW";
    case GL_STACK_UNDERFLOW: return "GL_STACK_UNDERFLOW";
    case GL_OUT_OF_MEMORY: return "GL_OUT_OF_MEMORY";
    case GL_INVALID_FRAMEBUFFER_OPERATION: return "GL_INVALID_FRAMEBUFFER_OPERATION";
    case GL_CONTEXT_LOST: return "GL_CONTEXT_LOST";
    default: DEBUG_UNREACHABLE(assert_handler{});
  }
  return "";
}

void opengl_check_error(const char* stmt, const char* file, unsigned int line) noexcept
{
  struct gl_assert_handler : debug_assert::default_handler, debug_assert::set_level<-1>
  {
    static void handle(const debug_assert::source_location&, const char*, const char* err_msg, const char* stmt, const char* file, unsigned int line)
    {
      debug_assert::source_location loc;
      loc.file_name = file;
      loc.line_number = line;

      try {
        std::string str = std::string(stmt) + " " + std::string(err_msg);

        debug_assert::default_handler::handle(loc, str.data());
      }
      catch (const std::exception& e) {
        DEBUG_UNREACHABLE(assert_handler{}, e.what());
        abort();
      }
    }
  };

  GLenum err;
  err = glGetError();
  DEBUG_ASSERT(err == GL_NO_ERROR, gl_assert_handler{}, opengl_err2str(err), stmt, file, line);
}

#endif // MINIGAME_SRC_THIRD_PARTY_OPENGL_HPP_INCLUDED
