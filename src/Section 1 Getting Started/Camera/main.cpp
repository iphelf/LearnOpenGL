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
  iphelf::opengl::Camera camera{create_camera(glm::vec3{0.0f}, {0, 0, 1})};

 public:
  Camera() : Application(800, 600, "Camera") {
    enable_depth_test();
    program.with_uniform("u_model2world", glm::mat4{1.0f});
    program.with_uniform("u_world2view", camera.world2view());
    program.with_uniform(
        "u_view2clip",
        glm::perspective(glm::radians(45.0), 800.0 / 600.0, 0.1, 100.0));
    enable_cursor_capture();
    add_cursor_pos_callback([this](double x, double y) {
      static double last_x{x};
      static double last_y{y};
      auto offset_x = static_cast<float>(x - last_x);
      auto offset_y = static_cast<float>(last_y - y);
      camera.rotate(offset_x, offset_y);
      last_x = x;
      last_y = y;
    });
  }

 private:
  void handle_inputs() {
    auto dt = delta_seconds();
    if (int forward =
            is_down(iphelf::opengl::Key::W) - is_down(iphelf::opengl::Key::S),
        right =
            is_down(iphelf::opengl::Key::D) - is_down(iphelf::opengl::Key::A),
        up = is_down(iphelf::opengl::Key::E) - is_down(iphelf::opengl::Key::Q);
        forward || right || up) {
      auto wasd{glm::normalize(glm::vec3{forward, right, up})};
      wasd *= dt;
      camera.move(wasd.x, wasd.y, wasd.z);
    }
  }

  void render() override {
    handle_inputs();

    clear(iphelf::opengl::Color::DarkGreenBluish);

    program.bind_texture(image);
    program.with_uniform("u_world2view", camera.world2view());
    program.render(cube);
  }
};

int main() {
  Camera application;
  application.run();
}