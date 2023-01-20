#pragma once

#include <iphelf/opengl/input.h>

#include <memory>

#include "gl.h"
#include "glfw.h"

struct GLFWwindow;

namespace iphelf::opengl {

class Window {
  struct Impl;
  std::unique_ptr<Impl> self;

 public:
  Window(int width, int height, const std::string &title);
  ~Window();
  Window(Window &&other) noexcept;
  Window &operator=(Window &&other) noexcept;

  [[nodiscard]] int width() const;
  [[nodiscard]] int height() const;
  using FramebufferSizeCallback = std::function<void(int width, int height)>;
  void add_framebuffer_size_callback(FramebufferSizeCallback &&callback);
  void enable_cursor_capture(bool enabled = true);
  using CursorPosCallback = std::function<void(double x, double y)>;
  void add_cursor_pos_callback(CursorPosCallback &&callback);
  using ScrollCallback = std::function<void(double offset_x, double offset_y)>;
  void add_scroll_callback(ScrollCallback &&callback);
  void set_should_close();
  [[nodiscard]] bool should_close() const;
  [[nodiscard]] bool is_down(Key key) const;
  [[nodiscard]] bool is_down(MouseButton mouse_button) const;
  void swap_buffers();
};

}  // namespace iphelf::opengl
