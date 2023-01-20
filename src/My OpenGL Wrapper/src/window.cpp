#include "window.h"

namespace iphelf::opengl {

struct Window::Impl {
 private:
  // Callbacks on framebuffer size change
  static std::unordered_map<GLFWwindow *, std::vector<FramebufferSizeCallback>>
      s_fs_callbacks;
  static void global_framebuffer_size_callback(GLFWwindow *w, int width,
                                               int height) {
    for (auto &callback : s_fs_callbacks[w]) callback(width, height);
  }
  static void add_global_framebuffer_size_callback(
      GLFWwindow *w, FramebufferSizeCallback &&callback) {
    auto found = s_fs_callbacks.find(w);
    if (found == s_fs_callbacks.end()) {
      glfw().set_framebuffer_size_callback(w, global_framebuffer_size_callback);
      s_fs_callbacks[w].push_back(callback);
    } else
      found->second.push_back(callback);
  }
  static void remove_global_framebuffer_size_callback(GLFWwindow *w) {
    s_fs_callbacks.erase(w);
  }

  // Callbacks on cursor position change
  static std::unordered_map<GLFWwindow *, std::vector<CursorPosCallback>>
      s_cp_callbacks;
  static void global_cursor_pos_callback(GLFWwindow *w, double x, double y) {
    for (auto &callback : s_cp_callbacks[w]) callback(x, y);
  }
  static void add_global_cursor_pos_callback(GLFWwindow *w,
                                             CursorPosCallback &&callback) {
    auto found = s_cp_callbacks.find(w);
    if (found == s_cp_callbacks.end()) {
      glfw().set_cursor_pos_callback(w, global_cursor_pos_callback);
      s_cp_callbacks[w].push_back(callback);
    } else
      found->second.push_back(callback);
  }
  static void remove_global_cursor_pos_callback(GLFWwindow *w) {
    s_cp_callbacks.erase(w);
  }

  // Callbacks on scroll change
  static std::unordered_map<GLFWwindow *, std::vector<ScrollCallback>>
      s_scroll_callbacks;
  static void global_scroll_callback(GLFWwindow *w, double offset_x,
                                     double offset_y) {
    for (auto &callback : s_scroll_callbacks[w]) callback(offset_x, offset_y);
  }
  static void add_global_scroll_callback(GLFWwindow *w,
                                         ScrollCallback &&callback) {
    auto found = s_scroll_callbacks.find(w);
    if (found == s_scroll_callbacks.end()) {
      glfw().set_scroll_callback(w, global_scroll_callback);
      s_scroll_callbacks[w].push_back(callback);
    } else
      found->second.push_back(callback);
  }
  static void remove_global_scroll_callback(GLFWwindow *w) {
    s_scroll_callbacks.erase(w);
  }

 public:
  GLFWwindow *w;
  int width;
  int height;
  Impl(int width, int height, const std::string &title)
      : w{glfw().create_window(width, height, title)},
        width{width},
        height{height} {
    add_global_framebuffer_size_callback(w, [this](int width, int height) {
      struct {
        int left = 0;
        int top = 0;
        int right = 0;
        int bottom = 0;
      } const p;
      gl().viewport(p.left, p.bottom, width - p.left - p.right,
                    height - p.top - p.bottom);
      this->width = width;
      this->height = height;
    });
  }
  ~Impl() {
    remove_global_cursor_pos_callback(w);
    remove_global_framebuffer_size_callback(w);
    glfw().destroy_window(w);
  }
  void add_framebuffer_size_callback(FramebufferSizeCallback &&callback) {
    add_global_framebuffer_size_callback(w, std::move(callback));
  }
  void add_cursor_pos_callback(CursorPosCallback &&callback) {
    add_global_cursor_pos_callback(w, std::move(callback));
  }
  void add_scroll_callback(ScrollCallback &&callback) {
    add_global_scroll_callback(w, std::move(callback));
  }
};

std::unordered_map<GLFWwindow *, std::vector<Window::FramebufferSizeCallback>>
    Window::Impl::s_fs_callbacks{};
std::unordered_map<GLFWwindow *, std::vector<Window::CursorPosCallback>>
    Window::Impl::s_cp_callbacks{};
std::unordered_map<GLFWwindow *, std::vector<Window::ScrollCallback>>
    Window::Impl::s_scroll_callbacks{};

Window::Window(int width, int height, const std::string &title)
    : self{std::make_unique<Impl>(width, height, title)} {}

Window::~Window() = default;

Window::Window(Window &&other) noexcept : self{nullptr} {
  *this = std::move(other);
}

Window &Window::operator=(Window &&other) noexcept {
  std::swap(self, other.self);
  return *this;
}

int Window::width() const { return self->width; }

int Window::height() const { return self->height; }

void Window::add_framebuffer_size_callback(
    std::function<void(int, int)> &&callback) {
  self->add_framebuffer_size_callback(std::move(callback));
}

void Window::enable_cursor_capture(bool enabled) {
  glfw().enable_cursor_capture(self->w, enabled);
}

void Window::add_cursor_pos_callback(CursorPosCallback &&callback) {
  self->add_cursor_pos_callback(std::move(callback));
}

void Window::add_scroll_callback(Window::ScrollCallback &&callback) {
  self->add_scroll_callback(std::move(callback));
}

void Window::set_should_close() { glfw().set_window_should_close(self->w); }

bool Window::should_close() const {
  return glfw().window_should_close(self->w);
}

bool Window::is_down(Key key) const { return glfw().is_key_down(self->w, key); }

bool Window::is_down(MouseButton mouse_button) const {
  return glfw().is_mouse_button_down(self->w, mouse_button);
}

void Window::swap_buffers() { glfw().swap_buffers(self->w); }

}  // namespace iphelf::opengl