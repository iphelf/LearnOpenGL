#include <iphelf/opengl/application.h>

#include <functional>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

const auto path_shaders{std::filesystem::current_path() / "shaders"};
const auto path_textures{std::filesystem::current_path() / "textures"};

class Camera : public iphelf::opengl::Application {
  const iphelf::opengl::Program program{create_program(
      path_shaders / "pos_uv.v.glsl", path_shaders / "image.f.glsl")};
  const iphelf::opengl::Texture image{
      create_texture(path_textures / "awesomeface.png")};
  const iphelf::opengl::TriangleArray cube{std::invoke([] {
    return create_triangle_array<5>(
        {
            {{-0.5f, -0.5f, -0.5f, 0.0f, 0.0f},
             {0.5f, -0.5f, -0.5f, 1.0f, 0.0f},
             {0.5f, 0.5f, -0.5f, 1.0f, 1.0f}},
            {{0.5f, 0.5f, -0.5f, 1.0f, 1.0f},
             {-0.5f, 0.5f, -0.5f, 0.0f, 1.0f},
             {-0.5f, -0.5f, -0.5f, 0.0f, 0.0f}},
            {{-0.5f, -0.5f, 0.5f, 0.0f, 0.0f},
             {0.5f, -0.5f, 0.5f, 1.0f, 0.0f},
             {0.5f, 0.5f, 0.5f, 1.0f, 1.0f}},
            {{0.5f, 0.5f, 0.5f, 1.0f, 1.0f},
             {-0.5f, 0.5f, 0.5f, 0.0f, 1.0f},
             {-0.5f, -0.5f, 0.5f, 0.0f, 0.0f}},
            {{-0.5f, 0.5f, 0.5f, 1.0f, 0.0f},
             {-0.5f, 0.5f, -0.5f, 1.0f, 1.0f},
             {-0.5f, -0.5f, -0.5f, 0.0f, 1.0f}},
            {{-0.5f, -0.5f, -0.5f, 0.0f, 1.0f},
             {-0.5f, -0.5f, 0.5f, 0.0f, 0.0f},
             {-0.5f, 0.5f, 0.5f, 1.0f, 0.0f}},
            {{0.5f, 0.5f, 0.5f, 1.0f, 0.0f},
             {0.5f, 0.5f, -0.5f, 1.0f, 1.0f},
             {0.5f, -0.5f, -0.5f, 0.0f, 1.0f}},
            {{0.5f, -0.5f, -0.5f, 0.0f, 1.0f},
             {0.5f, -0.5f, 0.5f, 0.0f, 0.0f},
             {0.5f, 0.5f, 0.5f, 1.0f, 0.0f}},
            {{-0.5f, -0.5f, -0.5f, 0.0f, 1.0f},
             {0.5f, -0.5f, -0.5f, 1.0f, 1.0f},
             {0.5f, -0.5f, 0.5f, 1.0f, 0.0f}},
            {{0.5f, -0.5f, 0.5f, 1.0f, 0.0f},
             {-0.5f, -0.5f, 0.5f, 0.0f, 0.0f},
             {-0.5f, -0.5f, -0.5f, 0.0f, 1.0f}},
            {{-0.5f, 0.5f, -0.5f, 0.0f, 1.0f},
             {0.5f, 0.5f, -0.5f, 1.0f, 1.0f},
             {0.5f, 0.5f, 0.5f, 1.0f, 0.0f}},
            {{0.5f, 0.5f, 0.5f, 1.0f, 0.0f},
             {-0.5f, 0.5f, 0.5f, 0.0f, 0.0f},
             {-0.5f, 0.5f, -0.5f, 0.0f, 1.0f}},
        },
        {3, 2});
  })};
  const float velocity{1.0f};
  iphelf::opengl::Camera camera{
      create_camera(glm::vec3{0.0f}, {0, 0, 1}, {0, 1, 0})};
  bool camera_moving{false};
  const double default_fov{45};
  double fov{default_fov};

 public:
  Camera() : Application(800, 600, "Camera") {
    enable_depth_test();
    program.with_uniform("u_model2world", glm::mat4{1.0f});
    program.with_uniform("u_world2view", camera.world2view());
    program.with_uniform(
        "u_view2clip",
        glm::perspective(glm::radians(45.0), 800.0 / 600.0, 0.1, 100.0));
    add_cursor_pos_callback([this](double x, double y) {
      static double last_x{x};
      static double last_y{y};
      auto offset_x = static_cast<float>(x - last_x);
      auto offset_y = static_cast<float>(last_y - y);
      if (camera_moving) camera.rotate(offset_x, offset_y);
      last_x = x;
      last_y = y;
    });
    add_scroll_callback([this](double, double offset_y) {
      const double sensitivity{5};
      std::cout << offset_y << '\r' << std::flush;
      fov = std::clamp(fov - offset_y * sensitivity, 5.0, 90.0);
    });
  }

 private:
  void handle_inputs() {
    camera_moving = is_down(iphelf::opengl::MouseButton::R);
    if (camera_moving) {
      if (just_pressed(iphelf::opengl::MouseButton::R)) enable_cursor_capture();
      auto dt{delta_seconds()};
      auto v{dt * velocity};
      if (int forward =
              is_down(iphelf::opengl::Key::W) - is_down(iphelf::opengl::Key::S),
          right =
              is_down(iphelf::opengl::Key::D) - is_down(iphelf::opengl::Key::A);
          forward || right) {
        auto wasd{glm::normalize(glm::vec2{forward, right})};
        wasd *= v;
        camera.move(wasd.x, wasd.y);
      }
      if (int up =
              is_down(iphelf::opengl::Key::E) - is_down(iphelf::opengl::Key::Q))
        camera.ascend(static_cast<float>(up) * v);
    } else if (just_released(iphelf::opengl::MouseButton::R))
      enable_cursor_capture(false);
    if (just_released(iphelf::opengl::MouseButton::M)) fov = default_fov;
  }

  void render() override {
    handle_inputs();

    clear(iphelf::opengl::Color::DarkGreenBluish);

    program.bind_texture(image);
    program.with_uniform(
        "u_view2clip",
        glm::perspective(glm::radians(fov), 800.0 / 600.0, 0.1, 100.0));
    program.with_uniform("u_world2view", camera.world2view());
    program.render(cube);
  }
};

int main() {
  Camera application;
  application.run();
}