#include <iphelf/opengl/application.h>

#include <unordered_map>

#include "gl.h"
#include "window.h"

namespace iphelf::opengl {

struct Application::Impl {
  Window window;
  static std::vector<CursorPosCallback>
      cursor_pos_callbacks;  // rough workaround to eliminate capturing
  std::chrono::duration<double> delta_time{};
  explicit Impl(Window &&window) : window{std::move(window)} {
    this->window.set_cursor_pos_callback([](GLFWwindow *, double x, double y) {
      for (auto &callback : cursor_pos_callbacks) callback(x, y);
    });
  }
};
std::vector<Application::CursorPosCallback>
    Application::Impl::cursor_pos_callbacks;

Application::Application(int width, int height, const std::string &title)
    : self(std::make_unique<Impl>(Window{width, height, title})) {}

Application::~Application() = default;

void Application::run() {
  std::chrono::duration<double> last_frame{-1.0 / 60.0};
  while (!self->window.should_close()) {
    if (self->window.is_down(Key::Escape)) self->window.set_should_close();
    auto curr_frame = glfw().get_time();
    self->delta_time = curr_frame - last_frame;
    last_frame = curr_frame;
    render();
    self->window.swap_buffers();
    glfw().poll_events();
  }
}

bool Application::is_down(Key key) { return self->window.is_down(key); }

bool Application::just_released(Key key) {
  static std::unordered_map<Key, bool> key_pressed;
  bool pressed_before = key_pressed[key];
  bool pressed_now = is_down(key);
  key_pressed[key] = pressed_now;
  return pressed_before && !pressed_now;
}

void Application::enable_cursor_capture(bool enabled) {
  self->window.enable_cursor_capture(enabled);
}

void Application::add_cursor_pos_callback(
    std::function<void(double, double)> &&callback) {
  self->cursor_pos_callbacks.push_back(callback);
}

float Application::elapsed_seconds() {
  return static_cast<float>(glfw().get_time().count());
}

float Application::delta_seconds() {
  return static_cast<float>(self->delta_time.count());
}

void Application::clear(const Color &color) {
  gl().set_clear_color(color);
  gl().clear();
}

void Application::enable_depth_test(bool enabled) {
  gl().enable_depth_test(enabled);
}

}  // namespace iphelf::opengl
