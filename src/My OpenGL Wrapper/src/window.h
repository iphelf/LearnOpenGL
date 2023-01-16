#pragma once

#include <iphelf/opengl/input.h>

#include "gl.h"
#include "glfw.h"

struct GLFWwindow;

namespace iphelf::opengl {

class Window {
  GLFWwindow *w;

 public:
  Window(int width, int height, const std::string &title)
      : w{glfw().create_window(width, height, title)} {
    // Setup OpenGL viewport inside the GLFW window
    auto framebuffer_size_callback = [](GLFWwindow *, int width, int height) {
      struct {
        int left = 0;
        int top = 0;
        int right = 0;
        int bottom = 0;
      } const p;
      gl().viewport(p.left, p.bottom, width - p.left - p.right,
                    height - p.top - p.bottom);
      // TODO: update width and height
    };
    framebuffer_size_callback(nullptr, width, height);
    set_framebuffer_size_callback(framebuffer_size_callback);
  }
  ~Window() {
    if (w) glfw().destroy_window(w);
  }
  Window(Window &&other) noexcept : w{nullptr} { std::swap(w, other.w); }

  void set_framebuffer_size_callback(
      GLFW::framebuffer_size_callback_t callback) {
    glfw().set_framebuffer_size_callback(w, callback);
  }
  void enable_cursor_capture(bool enabled = true) {
    glfw().enable_cursor_capture(w, enabled);
  }
  void set_cursor_pos_callback(GLFW::cursor_pos_callback_t callback) {
    glfw().set_cursor_pos_callback(w, callback);
  }
  void set_should_close() { glfw().set_window_should_close(w); }
  bool should_close() { return glfw().window_should_close(w); }
  bool is_down(Key key) { return glfw().is_key_down(w, key); }
  void swap_buffers() { glfw().swap_buffers(w); }
};

}  // namespace iphelf::opengl
