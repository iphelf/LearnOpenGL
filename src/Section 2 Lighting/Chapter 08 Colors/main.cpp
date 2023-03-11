#include <iphelf/opengl/application.h>

#include <functional>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

const auto path_shaders{std::filesystem::current_path() / "shaders"};

class Colors : public iphelf::opengl::Application {
  iphelf::opengl::Camera camera{
      create_camera(glm::vec3{2.0f, -2.0f, 2.0f},
                    {{0.0f, 0.0f, 1.0f}, {-1.0f, 1.0f, -1.0f}, 0.0f, -20.0f})};
  const iphelf::opengl::Program program_light{create_program(
      path_shaders / "light.v.glsl", path_shaders / "light.f.glsl")};
  const iphelf::opengl::Program program_object{create_program(
      path_shaders / "object.v.glsl", path_shaders / "object.f.glsl")};
  const iphelf::opengl::TriangleArray cube{create_triangle_array<3>({
      {{-0.5f, -0.5f, -0.5f}, {0.5f, -0.5f, -0.5f}, {0.5f, 0.5f, -0.5f}},
      {{0.5f, 0.5f, -0.5f}, {-0.5f, 0.5f, -0.5f}, {-0.5f, -0.5f, -0.5f}},
      {{-0.5f, -0.5f, 0.5f}, {0.5f, -0.5f, 0.5f}, {0.5f, 0.5f, 0.5f}},
      {{0.5f, 0.5f, 0.5f}, {-0.5f, 0.5f, 0.5f}, {-0.5f, -0.5f, 0.5f}},
      {{-0.5f, 0.5f, 0.5f}, {-0.5f, 0.5f, -0.5f}, {-0.5f, -0.5f, -0.5f}},
      {{-0.5f, -0.5f, -0.5f}, {-0.5f, -0.5f, 0.5f}, {-0.5f, 0.5f, 0.5f}},
      {{0.5f, 0.5f, 0.5f}, {0.5f, 0.5f, -0.5f}, {0.5f, -0.5f, -0.5f}},
      {{0.5f, -0.5f, -0.5f}, {0.5f, -0.5f, 0.5f}, {0.5f, 0.5f, 0.5f}},
      {{-0.5f, -0.5f, -0.5f}, {0.5f, -0.5f, -0.5f}, {0.5f, -0.5f, 0.5f}},
      {{0.5f, -0.5f, 0.5f}, {-0.5f, -0.5f, 0.5f}, {-0.5f, -0.5f, -0.5f}},
      {{-0.5f, 0.5f, -0.5f}, {0.5f, 0.5f, -0.5f}, {0.5f, 0.5f, 0.5f}},
      {{0.5f, 0.5f, 0.5f}, {-0.5f, 0.5f, 0.5f}, {-0.5f, 0.5f, -0.5f}},
  })};
  const glm::mat4 light_model2world{std::invoke([] {
    glm::mat4 model2world{1.0f};
    model2world = glm::translate(model2world, glm::vec3{0.0f, 0.0f, 2.0f});
    model2world = glm::scale(model2world, glm::vec3{0.2f});
    return model2world;
  })};
  const glm::mat4 object_model2world{1.0f};
  const iphelf::opengl::Color object_color{iphelf::opengl::Colors::Orange};

 public:
  Colors() : Application(800, 600, "Colors") {
    enable_depth_test();
    bind_default_camera_controller(camera);
    program_light.with_uniform("u_model2world", light_model2world);
    program_object.with_uniform("u_object_color", object_color);
    program_object.with_uniform("u_model2world", object_model2world);
  }

 private:
  void render() override {
    clear(iphelf::opengl::Colors::DarkGreenBluish);
    auto view2clip{camera.view2clip()};
    auto world2view{camera.world2view()};
    static float hue{0.0f};
    auto light_color{iphelf::opengl::Color::from_hsv(hue, 1.0f, 1.0f)};
    program_light.with_uniform("u_light_color", light_color);
    program_light.with_uniform("u_world2view", world2view);
    program_light.with_uniform("u_view2clip", view2clip);
    program_light.render(cube);
    program_object.with_uniform("u_light_color", light_color);
    program_object.with_uniform("u_world2view", world2view);
    program_object.with_uniform("u_view2clip", view2clip);
    program_object.render(cube);
    hue += delta_seconds() * 60.0f;
    if (hue > 360.0f) hue -= 360.0f;
  }
};

int main() {
  Colors application;
  application.run();
}