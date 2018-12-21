#ifndef MINIGAME_SRC_GFX_GFX_DRAW_HPP_INCLUDED
#define MINIGAME_SRC_GFX_GFX_DRAW_HPP_INCLUDED

#include "../third_party/opengl.hpp"
#include "../third_party/debug_assert.hpp"

#include <vector>
#include <stdlib.h>
#include <stddef.h>


struct GfxDraw {

  struct Vertex {
    float x, y, u, v;
  };

  GLuint vao;
  GLuint vbo;
  std::vector<GfxTexture*> textures;
  std::vector<GfxDraw::Vertex> vertices;

  void create(int32_t num_vertex) noexcept;
  void destroy() noexcept;
  void draw_texture(GfxTexture* tex, float x, float y) noexcept;
  void draw() noexcept;

};

void GfxDraw::create(int32_t num_quad) noexcept
{
  GL_CHECK(glGenVertexArrays(1, &this->vao));
  GL_CHECK(glBindVertexArray(this->vao));
  DEBUG_ASSERT(glIsVertexArray(this->vao), assert_handler{});
  GL_CHECK(glEnableVertexAttribArray(0));
  GL_CHECK(glEnableVertexAttribArray(1));

  GLsizeiptr bytes = sizeof(GfxDraw::Vertex) * num_quad * 6;

  GL_CHECK(glGenBuffers(1, &this->vbo));
  GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, this->vbo));
  DEBUG_ASSERT(glIsBuffer(this->vbo), assert_handler{});
  GL_CHECK(glBufferData(GL_ARRAY_BUFFER, bytes, nullptr, GL_STREAM_DRAW));

  GLint n;
  GL_CHECK(glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &n));
  DEBUG_ASSERT(n == bytes, assert_handler{});

  GL_CHECK(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)0));
  GL_CHECK(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)offsetof(GfxDraw::Vertex, u)));

  try {
    this->textures.reserve(num_quad);
    this->vertices.reserve(num_quad*6);
  }
  catch (std::exception& e) {
    DEBUG_UNREACHABLE(assert_handler{}, e.what());
    abort();
  }
}

void GfxDraw::destroy() noexcept
{
  GL_CHECK(glDeleteBuffers(1, &this->vbo));
  GL_CHECK(glDeleteVertexArrays(1, &this->vao));
}

void GfxDraw::draw_texture(GfxTexture* tex, float x, float y) noexcept
{
  GfxDraw::Vertex tl, tr, bl, br;
  tl.x = x;
  tl.y = y;
  tl.u = 0.0f;
  tl.v = 0.0f;
  tr.x = x + tex->w;
  tr.y = y;
  tr.u = 1.0f;
  tr.v = 0.0f;
  bl.x = x;
  bl.y = y + tex->h;
  bl.u = 0.0f;
  bl.v = 1.0f;
  br.x = x + tex->w;
  br.y = y + tex->h;
  br.u = 1.0f;
  br.v = 1.0f;

  try {
    this->textures.push_back(tex);

    this->vertices.push_back(tl);
    this->vertices.push_back(bl);
    this->vertices.push_back(br);
    this->vertices.push_back(br);
    this->vertices.push_back(tr);
    this->vertices.push_back(tl);
  }
  catch (std::exception& e) {
    DEBUG_UNREACHABLE(assert_handler{}, e.what());
    abort();
  }
}

void GfxDraw::draw() noexcept
{
  GLsizeiptr bytes = sizeof(GfxDraw::Vertex) * this->vertices.size();

  GL_CHECK(glBindVertexArray(this->vao));
  GL_CHECK(glBufferData(GL_ARRAY_BUFFER, bytes, nullptr, GL_STREAM_DRAW));
  GL_CHECK(glBufferData(GL_ARRAY_BUFFER, bytes, this->vertices.data(), GL_STREAM_DRAW));

  GfxTexture* prev_tex = nullptr;
  for (GfxTexture* tex : this->textures) {
    if (prev_tex != tex) { tex->bind(); }
    GL_CHECK(glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(this->vertices.size())));
    prev_tex = tex;
  }

  this->textures.clear();
  this->vertices.clear();
}

#endif // MINIGAME_SRC_GFX_GFX_DRAW_HPP_INCLUDED
