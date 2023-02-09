#include <iphelf/opengl/application.h>

#include <functional>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

const auto path_shaders{std::filesystem::current_path() / "shaders"};
const auto path_textures{std::filesystem::current_path() / "textures"};

class Camera : public iphelf::opengl::Application {
  const iphelf::opengl::Program program{create_program(
      path_shaders / "transform_uv.v.glsl", path_shaders / "image.f.glsl")};
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
  iphelf::opengl::Camera camera{
      create_camera(glm::vec3{0.0f}, {0, 0, 1}, {0, 1, 0})};

 public:
  Camera() : Application(800, 600, "Camera") {
    enable_depth_test();
    program.with_uniform("u_model2world", glm::mat4{1.0f});
    bind_default_camera_controller(camera);
  }

 private:
  void render() override {
    clear(iphelf::opengl::Colors::DarkGreenBluish);

    program.bind_texture(image);
    program.with_uniform("u_view2clip",
                         glm::perspective(glm::radians(camera.fov()),
                                          800.0 / 600.0, 0.1, 100.0));
    program.with_uniform("u_world2view", camera.world2view());
    program.render(cube);
  }
};

int main() {
  Camera application;
  application.run();
}