#include <iphelf/imgui/instance.h>
#include <iphelf/opengl/application.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "lights.h"

const auto path_shaders{std::filesystem::current_path() / "shaders"};
const auto path_textures{std::filesystem::current_path() / "textures"};

struct Material {
  const iphelf::opengl::Texture diffuse;
  const iphelf::opengl::Texture specular;
  float shininess;
  void set_uniform(const iphelf::opengl::Program &program) const {
    program.bind_texture("u_material.diffuse", 0, diffuse);
    program.bind_texture("u_material.specular", 1, specular);
    program.with_uniform("u_material.shininess", shininess);
  }
};

class LightCasters : public iphelf::opengl::Application {
  const iphelf::opengl::Program program_light{create_program(
      path_shaders / "light.v.glsl", path_shaders / "light.f.glsl")};

  const iphelf::opengl::TriangleArray cube{std::invoke([] {
    return create_triangle_array<8>(
        {
            {{-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f},
             {0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f},
             {0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f}},
            {{0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f},
             {-0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f},
             {-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f}},
            {{-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f},
             {0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f},
             {0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f}},
            {{0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f},
             {-0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f},
             {-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f}},
            {{-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f},
             {-0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f},
             {-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f}},
            {{-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f},
             {-0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f},
             {-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f}},
            {{0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f},
             {0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f},
             {0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f}},
            {{0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f},
             {0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f},
             {0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f}},
            {{-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f},
             {0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f},
             {0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f}},
            {{0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f},
             {-0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f},
             {-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f}},
            {{-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f},
             {0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f},
             {0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f}},
            {{0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f},
             {-0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f},
             {-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f}},
        },
        {3, 3, 2});  // (position, normal, uv)
  })};
  Material material{
      .diffuse = create_texture(path_textures / "container2.png"),
      .specular = create_texture(path_textures / "container2_specular.png"),
      .shininess = 32.0f,
  };

  const std::vector<glm::mat4> cube_model2worlds{std::invoke([] {
    std::vector<glm::mat4> model2worlds;
    std::vector<glm::vec3> positions{
        {0.0f, 0.0f, 0.0f},     {2.0f, 5.0f, -15.0f}, {-1.5f, -2.2f, -2.5f},
        {-3.8f, -2.0f, -12.3f}, {2.4f, -0.4f, -3.5f}, {-1.7f, 3.0f, -7.5f},
        {1.3f, -2.0f, -2.5f},   {1.5f, 2.0f, -2.5f},  {1.5f, 0.2f, -1.5f},
        {-1.3f, 1.0f, -1.5f},
    };
    model2worlds.reserve(positions.size());
    {
      float angle{0.0f};
      const glm::vec3 axis{1.0f, 0.3f, 0.5f};
      for (const auto &position : positions) {
        glm::mat4 model2world{1.0f};
        model2world = glm::translate(model2world, position);
        model2world = glm::rotate(model2world, glm::radians(angle), axis);
        model2worlds.push_back(model2world);
        angle += 20.0f;
      }
    }
    return model2worlds;
  })};
  const std::vector<glm::mat3> cube_model2worlds_normalized{
      std::invoke([&model2worlds = cube_model2worlds] {
        std::vector<glm::mat3> model2worlds_normalized;
        model2worlds_normalized.resize(model2worlds.size());
        std::transform(
            model2worlds.begin(), model2worlds.end(),
            model2worlds_normalized.begin(), [](const auto &model2world) {
              return glm::transpose(glm::inverse(glm::mat3{model2world}));
            });
        return model2worlds_normalized;
      })};

  struct CameraState {
    glm::vec3 position{0.0f};
    float yaw{0.0f};
    float pitch{0.0f};
  };
  CameraState camera_state_view{{2.0f, -2.0f, 2.0f}, 0.0f, -20.0f};
  CameraState camera_state_light{{0.0f, 0.0f, 2.0f}, 0.0f, -90.0f};
  glm::mat4 light_model2world{
      {0.2f, 0.0f, 0.0f, 0.0f},
      {0.0f, 0.2f, 0.0f, 0.0f},
      {0.0f, 0.0f, 0.2f, 0.0f},
      {0.0f, 0.0f, 2.0f, 1.0f},
  };
  PointLight point_light{
      create_program(path_shaders / "object.v.glsl",
                     path_shaders / "object_point_light.f.glsl")};
  DirectionalLight dir_light{
      create_program(path_shaders / "object.v.glsl",
                     path_shaders / "object_dir_light.f.glsl")};
  SpotLight spot_light{
      create_program(path_shaders / "object.v.glsl",
                     path_shaders / "object_spot_light.f.glsl")};
  int light_focused{0};
  std::vector<Light *> lights{&point_light, &dir_light, &spot_light};
  bool view_focused{true};
  iphelf::opengl::Camera camera{
      create_camera(camera_state_view.position, {{0.0f, 0.0f, 1.0f},
                                                 {-1.0f, 1.0f, -1.0f},
                                                 camera_state_view.yaw,
                                                 camera_state_view.pitch})};

  iphelf::imgui::Instance imgui{get_glfw_window()};

 public:
  LightCasters() : Application(800, 600, "Light casters") {
    enable_depth_test();
    bind_default_camera_controller(camera);
    imgui.install_callbacks();

    point_light.position = camera_state_light.position;
    dir_light.direction = glm::vec3{0.0f, 0.0f, -1.0f};
    spot_light.position = point_light.position;
    spot_light.direction = dir_light.direction;
  }

 private:
  void render() override {
    clear(iphelf::opengl::Colors::Dark);

    auto view2clip{camera.view2clip()};
    auto world2view{camera.world2view()};

    if (view_focused) {
      program_light.with_uniform("u_model2world", light_model2world);
      program_light.with_uniform("u_world2view", world2view);
      program_light.with_uniform("u_view2clip", view2clip);
      program_light.with_uniform("u_light_color", point_light.color);
      program_light.render(cube);
    } else {
      light_model2world = glm::scale(camera.model2world(), glm::vec3{0.2f});
      point_light.position = camera.position();
      dir_light.direction =
          glm::mat3{camera.model2world()} * glm::vec3{0.0f, 0.0f, -1.0f};
      spot_light.position = point_light.position;
      spot_light.direction = dir_light.direction;
    }

    auto &light{lights[light_focused]};
    light->program.with_uniform("u_world2view", world2view);
    light->program.with_uniform("u_view2clip", view2clip);
    light->set_uniform();
    material.set_uniform(light->program);
    for (std::size_t i{0}; i < cube_model2worlds.size(); ++i) {
      light->program.with_uniform("u_model2world", cube_model2worlds[i]);
      light->program.with_uniform("u_model2world_normalized",
                                  cube_model2worlds_normalized[i]);
      light->program.render(cube);
    }

    // (TAB key) switch view/control between camera and light
    if (just_released(iphelf::opengl::Key::Tab)) {
      auto &camera_state_prev =
          view_focused ? camera_state_view : camera_state_light;
      camera_state_prev.position = camera.position();
      auto [yaw, pitch] = camera.rotation();
      camera_state_prev.yaw = yaw;
      camera_state_prev.pitch = pitch;
      view_focused ^= true;
      auto &camera_state_curr =
          view_focused ? camera_state_view : camera_state_light;
      camera.set_position(camera_state_curr.position);
      camera.set_rotation(camera_state_curr.yaw, camera_state_curr.pitch);
    }

    // (Number key) switch between point, directional, and spot, light
    if (just_released(iphelf::opengl::Key::_1))
      light_focused = 0;
    else if (just_released(iphelf::opengl::Key::_2))
      light_focused = 1;
    else if (just_released(iphelf::opengl::Key::_3))
      light_focused = 2;
  }
};

int main() {
  LightCasters application;
  application.run();
}