#ifndef MINIGAME_SRC_WINDOW_WINDOW_HPP_INCLUDED
#define MINIGAME_SRC_WINDOW_WINDOW_HPP_INCLUDED

#include "../third_party/glfw.hpp"
#include "../third_party/debug_assert.hpp"


struct Window {

  GLFWwindow* window;

  void open()
  {
    this->window = glfwCreateWindow(640, 480, "libminigame", nullptr, nullptr);
    DEBUG_ASSERT(this->window, assert_handler{});
  }

  void close()
  {
    glfwDestroyWindow(this->window);
  }

};

#endif // MINIGAME_SRC_WINDOW_WINDOW_HPP_INCLUDED
