#include <iphelf/opengl/application.h>

#include <unordered_map>

#include "gl.h"
#include "window.h"

namespace iphelf::opengl {

struct Application::Impl {
  Window window;
  explicit Impl(Window &&window) : window{std::move(window)} {}
};

Application::Application(int width, int height, const std::string &title)
    : self(std::make_unique<Impl>(Window{width, height, title})) {}

Application::~Application() = default;

void Application::run() {
  while (!self->window.should_close()) {
    if (self->window.is_down(Key::Escape)) self->window.set_should_close();
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

float Application::elapsed_seconds() {
  return static_cast<float>(glfw().get_time().count());
}

void Application::clear(const Color &color) {
  gl().set_clear_color(color);
  gl().clear();
}

void Application::enable_depth_test(bool enabled) {
  gl().enable_depth_test(enabled);
}

}  // namespace iphelf::opengl
