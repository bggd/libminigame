#include "disable_warning_push.h"

#ifdef _WIN32

  #define _GLFW_WIN32

  #include "../../third_party/glfw/src/context.c"
  #include "../../third_party/glfw/src/init.c"
  #include "../../third_party/glfw/src/input.c"
  #include "../../third_party/glfw/src/monitor.c"
  #include "../../third_party/glfw/src/vulkan.c"
  #include "../../third_party/glfw/src/window.c"


  #include "../../third_party/glfw/src/win32_init.c"
  #include "../../third_party/glfw/src/win32_joystick.c"
  #include "../../third_party/glfw/src/win32_monitor.c"
  #include "../../third_party/glfw/src/win32_time.c"
  #include "../../third_party/glfw/src/win32_thread.c"
  #include "../../third_party/glfw/src/win32_window.c"
  #include "../../third_party/glfw/src/wgl_context.c"
  #include "../../third_party/glfw/src/egl_context.c"
  #include "../../third_party/glfw/src/osmesa_context.c"

#endif

#include "disable_warning_pop.h"
