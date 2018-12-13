#ifndef MINIGAME_TEST_GFX_TEST_GFX_SHADER_HPP_INCLUDED
#define MINIGAME_TEST_GFX_TEST_GFX_SHADER_HPP_INCLUDED

#include <gtest/gtest.h>

#include "../../src/window/window.hpp"
#include "../../src/gfx/gfx_shader.hpp"


TEST(GfxShader, Test)
{
  const char* vertex_glsl = R"(
#version 330
layout(location = 0) in vec2 pos;
layout(location = 1) in vec2 uv;
out vec2 v_uv;
void main()
{
  gl_Position = vec4(pos, 0.0, 1.0);
  v_uv = uv;
}
)";
  const char* fragment_glsl = R"(
#version 330
uniform sampler2D tex;
in vec2 v_uv;
out vec4 frag_color;
void main()
{
  frag_color = texture2D(tex, v_uv);
}
)";

  Window w;
  w.open();

  GfxShader shader;
  shader.create_from_memory(vertex_glsl, fragment_glsl);
  shader.destroy();

  w.close();
}

#endif // MINIGAME_TEST_GFX_TEST_GFX_SHADER_HPP_INCLUDED
