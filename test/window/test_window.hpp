#ifndef MINIGAME_TEST_WINDOW_TEST_WINDOW_HPP_INCLUDED
#define MINIGAME_TEST_WINDOW_TEST_WINDOW_HPP_INCLUDED

#include <gtest/gtest.h>

#include "../../src/window/window.hpp"
#include "../../src/third_party/debug_assert.hpp"


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

  Window w;

  w.open();
  w.close();

  glfwTerminate();
}

TEST(Window, Test)
{
  //test_window();
}

#endif // MINIGAME_TEST_WINDOW_TEST_WINDOW_HPP_INCLUDED
