#ifndef MINIGAME_TEST_GFX_TEST_GFX_SHADER_HPP_INCLUDED
#define MINIGAME_TEST_GFX_TEST_GFX_SHADER_HPP_INCLUDED

#include <gtest/gtest.h>

#include "../../src/window/window.hpp"
#include "../../src/gfx/gfx_shader.hpp"


TEST(GfxShader, Test)
{
  const char* vertex_glsl = R"(
#version 330
in vec2 pos;
void main()
{
  gl_Position = vec4(pos, 0.0, 1.0);
}
)";
  const char* fragment_glsl = R"(
#version 330
out vec4 fragColor;
void main()
{
  fragColor = vec4(1.0, 1.0, 1.0, 1.0);
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
