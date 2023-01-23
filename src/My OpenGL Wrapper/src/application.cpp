#include <iphelf/opengl/application.h>

#include <glm/glm.hpp>
#include <unordered_map>

#include "external/gl.h"
#include "window.h"

namespace iphelf::opengl {

struct Application::Impl {
  Window window;
  std::chrono::duration<double> delta_time{};
  struct CameraController {
    Camera &camera;
    bool moving{false};
    const float velocity{1.0f};
    explicit CameraController(Camera &camera) : camera{camera} {}
    void cursor_pos_callback(double x, double y) {
      static double last_x{x};
      static double last_y{y};
      auto offset_x = static_cast<float>(x - last_x);
      auto offset_y = static_cast<float>(last_y - y);
      if (moving) camera.rotate(offset_x, offset_y);
      last_x = x;
      last_y = y;
    }
    void scroll_callback(double, double offset_y) { camera.zoom(offset_y); }
    void handle_inputs(Application &app) {
      moving = app.is_down(iphelf::opengl::MouseButton::R);
      if (moving) {
        if (app.just_pressed(iphelf::opengl::MouseButton::R))
          app.enable_cursor_capture();
        auto dt{app.delta_seconds()};
        auto v{dt * velocity};
        if (int forward = app.is_down(iphelf::opengl::Key::W) -
                          app.is_down(iphelf::opengl::Key::S),
            right = app.is_down(iphelf::opengl::Key::D) -
                    app.is_down(iphelf::opengl::Key::A);
            forward || right) {
          auto wasd{glm::normalize(glm::vec2{forward, right})};
          wasd *= v;
          camera.move(wasd.x, wasd.y);
        }
        if (int up = app.is_down(iphelf::opengl::Key::E) -
                     app.is_down(iphelf::opengl::Key::Q))
          camera.ascend(static_cast<float>(up) * v);
      } else if (app.just_released(iphelf::opengl::MouseButton::R))
        app.enable_cursor_capture(false);
      if (app.just_released(iphelf::opengl::MouseButton::M))
        camera.reset_zoom();
    }
  };
  std::unique_ptr<CameraController> camera_controller{nullptr};

  explicit Impl(Window &&window) : window{std::move(window)} {}
};

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
    if (self->camera_controller) self->camera_controller->handle_inputs(*this);
    render();
    self->window.swap_buffers();
    glfw().poll_events();
  }
}

bool Application::is_down(Key key) const { return self->window.is_down(key); }

bool Application::is_down(MouseButton mouse_button) const {
  return self->window.is_down(mouse_button);
}

bool Application::just_changed_to(Key key, bool pressed) const {
  static std::unordered_map<Key, bool> key_pressed;
  bool pressed_before = key_pressed[key];
  bool pressed_now = is_down(key);
  key_pressed[key] = pressed_now;
  return pressed_before != pressed_now && pressed_now == pressed;
}

bool Application::just_changed_to(MouseButton mouse_button,
                                  bool pressed) const {
  static std::unordered_map<MouseButton, bool> mouse_button_pressed;
  bool pressed_before = mouse_button_pressed[mouse_button];
  bool pressed_now = is_down(mouse_button);
  mouse_button_pressed[mouse_button] = pressed_now;
  return pressed_before != pressed_now && pressed_now == pressed;
}

void Application::enable_cursor_capture(bool enabled) {
  self->window.enable_cursor_capture(enabled);
}

void Application::add_cursor_pos_callback(
    Application::CursorPosCallback &&callback) {
  self->window.add_cursor_pos_callback(std::move(callback));
}

void Application::add_scroll_callback(Application::ScrollCallback &&callback) {
  self->window.add_scroll_callback(std::move(callback));
}

void Application::bind_default_camera_controller(Camera &camera) {
  self->camera_controller = std::make_unique<Impl::CameraController>(camera);
  add_cursor_pos_callback(
      [&camera_controller = self->camera_controller](double x, double y) {
        camera_controller->cursor_pos_callback(x, y);
      });
  add_scroll_callback([&camera_controller = self->camera_controller](
                          double offset_x, double offset_y) {
    camera_controller->scroll_callback(offset_x, offset_y);
  });
}

float Application::elapsed_seconds() {
  return static_cast<float>(glfw().get_time().count());
}

float Application::delta_seconds() const {
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
