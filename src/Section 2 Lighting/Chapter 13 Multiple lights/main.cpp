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

class MultipleLights : public iphelf::opengl::Application {
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

  iphelf::opengl::Program program_object{create_program(
      path_shaders / "object.v.glsl", path_shaders / "object.f.glsl")};
  PointLight point_light0{"u_point_lights[0]", glm::vec3{3.0f, 0.0f, 0.0f}};
  PointLight point_light1{"u_point_lights[1]", glm::vec3{0.0f, -3.0f, 0.0f}};
  PointLight point_light2{"u_point_lights[2]", glm::vec3{0.0f, 0.0f, 3.0f}};
  PointLight point_light3{"u_point_lights[3]", glm::vec3{3.0f, -3.0f, 3.0f}};
  DirectionalLight dir_light{"u_dir_light"};
  SpotLight spot_light{"u_spot_light", glm::vec3{2.0f, -2.0f, 2.0f}};
  const std::vector<glm::mat4> light_model2worlds{std::invoke([this] {
    std::vector<glm::mat4> model2worlds;
    const glm::mat4 identity{1.0f};
    model2worlds.push_back(glm::translate(identity, point_light0.position));
    model2worlds.push_back(glm::translate(identity, point_light1.position));
    model2worlds.push_back(glm::translate(identity, point_light2.position));
    model2worlds.push_back(glm::translate(identity, point_light3.position));
    model2worlds.push_back(
        glm::translate(identity, glm::vec3{0.0f, 0.0f, 10.0f}));
    model2worlds.push_back(glm::translate(identity, spot_light.position));
    for (auto &model2world : model2worlds)
      model2world = glm::scale(model2world, glm::vec3{0.2f});
    return model2worlds;
  })};
  std::vector<Light *> lights{
      &point_light0, &point_light1, &point_light2,
      &point_light3, &dir_light,    &spot_light,
  };
  iphelf::opengl::Camera camera{
      create_camera({2.0f, -2.0f, 2.0f},
                    {{0.0f, 0.0f, 1.0f}, {-1.0f, 1.0f, -1.0f}, 0.0f, -20.0f})};

  iphelf::imgui::Instance imgui{get_glfw_window()};

 public:
  MultipleLights() : Application(800, 600, "Multiple lights") {
    enable_depth_test();
    bind_default_camera_controller(camera);
    imgui.install_callbacks();
  }

 private:
  void render() override {
    clear(iphelf::opengl::Colors::Dark);

    auto view2clip{camera.view2clip()};
    auto world2view{camera.world2view()};

    program_light.with_uniform("u_world2view", world2view);
    program_light.with_uniform("u_view2clip", view2clip);
    program_light.with_uniform("u_light_color", iphelf::opengl::Colors::White);
    for (auto &model2world : light_model2worlds) {
      program_light.with_uniform("u_model2world", model2world);
      program_light.render(cube);
    }

    program_object.with_uniform("u_world2view", world2view);
    program_object.with_uniform("u_view2clip", view2clip);
    material.set_uniform(program_object);
    for (auto light : lights) light->set_uniform(program_object);
    for (std::size_t i{0}; i < cube_model2worlds.size(); ++i) {
      program_object.with_uniform("u_model2world", cube_model2worlds[i]);
      program_object.with_uniform("u_model2world_normalized",
                                  cube_model2worlds_normalized[i]);
      program_object.render(cube);
    }
  }
};

int main() {
  MultipleLights application;
  application.run();
}