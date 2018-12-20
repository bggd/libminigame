#ifndef MINIGAME_TEST_WINDOW_TEST_WINDOW_HPP_INCLUDED
#define MINIGAME_TEST_WINDOW_TEST_WINDOW_HPP_INCLUDED

#include <gtest/gtest.h>

#include "../../src/window/window.hpp"
#include "../../src/third_party/debug_assert.hpp"
#include "../../src/third_party/opengl.hpp"

#include <stdio.h>


void error_callback(int, const char* description)
{
  DEBUG_UNREACHABLE(assert_handler{}, description);
}

void test_window()
{
  glfwSetErrorCallback(error_callback);

  int err = glfwInit();
  ASSERT_TRUE(err == GLFW_TRUE);

  glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
  glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_OSMESA_CONTEXT_API);

  Window w;

  w.open();

  ASSERT_FALSE(w.has_close_request());

  ASSERT_TRUE(load_gl());

  printf("GL_VENDOR: %s\n", glGetString(GL_VENDOR));
  printf("GL_RENDERER: %s\n", glGetString(GL_RENDERER));
  printf("GL_VERSION: %s\n", glGetString(GL_VERSION));
  printf("GL_SHADING_LANGUAGE_VERSION: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

  glClear(GL_COLOR_BUFFER_BIT);
  w.swap_buffers();

  w.close();
}

TEST(Window, Test)
{
  test_window();
}

#endif // MINIGAME_TEST_WINDOW_TEST_WINDOW_HPP_INCLUDED
