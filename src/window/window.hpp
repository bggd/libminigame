#ifndef MINIGAME_SRC_WINDOW_WINDOW_HPP_INCLUDED
#define MINIGAME_SRC_WINDOW_WINDOW_HPP_INCLUDED

#include "../third_party/glfw.hpp"
#include "../third_party/debug_assert.hpp"


struct Window {

  GLFWwindow* window;

  void open()
  {
    //glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    this->window = glfwCreateWindow(640, 480, "", nullptr, nullptr);
    DEBUG_ASSERT(this->window, assert_handler{});
    glfwMakeContextCurrent(this->window);
  }

  void close()
  {
    glfwDestroyWindow(this->window);
  }

};

#endif // MINIGAME_SRC_WINDOW_WINDOW_HPP_INCLUDED
