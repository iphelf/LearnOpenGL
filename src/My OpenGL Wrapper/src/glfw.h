#pragma once

#include <iphelf/opengl/input.h>

#include <chrono>
#include <functional>
#include <string>

struct GLFWwindow;

namespace iphelf::opengl {

class GLFW {
  GLFW();
  ~GLFW();
  friend GLFW &glfw();

 public:
  using gl_proc = void (*)();
  gl_proc (*get_proc_address)(const char *);

  // window functions
  GLFWwindow *create_window(int width, int height, const std::string &title);
  using framebuffer_size_callback_t = void (*)(GLFWwindow *, int, int);
  void set_framebuffer_size_callback(GLFWwindow *w,
                                     framebuffer_size_callback_t callback);
  void destroy_window(GLFWwindow *w);
  bool window_should_close(GLFWwindow *w);
  void set_window_should_close(GLFWwindow *w);
  bool is_key_down(GLFWwindow *w, Key k);

  // render loop
  void swap_buffers(GLFWwindow *w);
  void poll_events();
  std::chrono::duration<double> get_time();
};

GLFW &glfw();

}  // namespace iphelf::opengl
