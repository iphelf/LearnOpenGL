#include <iphelf/imgui/instance.h>
#include <iphelf/opengl/application.h>

#include <functional>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

const auto path_shaders{std::filesystem::current_path() / "shaders"};

class Colors : public iphelf::opengl::Application {
  iphelf::opengl::Camera camera{
      create_camera(glm::vec3{2.0f, -2.0f, 2.0f}, {0.0f, 0.0f, 1.0f},
                    {-1.0f, 1.0f, -1.0f}, 0.0f, -20.0f)};
  const iphelf::opengl::Program program_light{create_program(
      path_shaders / "light.v.glsl", path_shaders / "light.f.glsl")};
  const iphelf::opengl::Program program_object{create_program(
      path_shaders / "object.v.glsl", path_shaders / "object.f.glsl")};
  const iphelf::opengl::TriangleArray cube{std::invoke([] {
    return create_triangle_array<6>(
        {
            {{-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f},
             {0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f},
             {0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f}},
            {{0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f},
             {-0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f},
             {-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f}},
            {{-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f},
             {0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f},
             {0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f}},
            {{0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f},
             {-0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f},
             {-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f}},
            {{-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f},
             {-0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f},
             {-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f}},
            {{-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f},
             {-0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f},
             {-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f}},
            {{0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f},
             {0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f},
             {0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f}},
            {{0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f},
             {0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f},
             {0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f}},
            {{-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f},
             {0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f},
             {0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f}},
            {{0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f},
             {-0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f},
             {-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f}},
            {{-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f},
             {0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f},
             {0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f}},
            {{0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f},
             {-0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f},
             {-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f}},
        },
        {3, 3});
  })};
  const glm::mat4 light_model2world{std::invoke([] {
    glm::mat4 model2world{1.0f};
    model2world = glm::translate(model2world, glm::vec3{0.0f, 2.0f, 2.0f});
    model2world = glm::scale(model2world, glm::vec3{0.1f});
    return model2world;
  })};
  const glm::mat4 object_model2world{1.0f};
  const iphelf::opengl::Color object_color{iphelf::opengl::Color::Orange};
  iphelf::imgui::Instance imgui{get_glfw_window()};

 public:
  Colors() : Application(800, 600, "Basic Lighting") {
    enable_depth_test();
    bind_default_camera_controller(camera);
    imgui.install_callbacks();
    program_object.with_uniform("u_object_color", object_color);
    program_object.with_uniform("u_model2world", object_model2world);
    program_object.with_uniform(
        "u_model2world_normalized",
        glm::mat3{glm::transpose(glm::inverse(object_model2world))});
    program_object.with_uniform("u_ambient_strength", 0.1f);
    program_object.with_uniform("u_diffuse_strength", 1.0f);
    program_object.with_uniform("u_specular_strength", 0.5f);
    program_object.with_uniform("u_shininess", 32.0f);
  }

 private:
  void render() override {
    clear(iphelf::opengl::Color::Black);

    auto view2clip{glm::perspective(glm::radians(camera.fov()), 800.0 / 600.0,
                                    0.1, 100.0)};
    auto world2view{camera.world2view()};
    static float phase{0.0f};
    auto light_color{iphelf::opengl::Color::from_hsv(phase, 1.0f, 1.0f)};
    auto rotated_light_model2world{glm::rotate(glm::mat4{1.0f},
                                               glm::radians(phase),
                                               glm::vec3{0.0f, 0.0f, 1.0f}) *
                                   light_model2world};

    program_light.with_uniform("u_light_color", light_color);
    program_light.with_uniform("u_model2world", rotated_light_model2world);
    program_light.with_uniform("u_world2view", world2view);
    program_light.with_uniform("u_view2clip", view2clip);
    program_light.render(cube);

    program_object.with_uniform("u_light_pos",
                                glm::vec3{rotated_light_model2world *
                                          glm::vec4{glm::vec3{0.0f}, 1.0f}});
    program_object.with_uniform("u_camera_pos", camera.pos());
    program_object.with_uniform("u_light_color", light_color);
    program_object.with_uniform("u_world2view", world2view);
    program_object.with_uniform("u_view2clip", view2clip);
    program_object.render(cube);

    static bool pause_phase{false};
    if (just_released(iphelf::opengl::Key::Space)) pause_phase = !pause_phase;
    if (!pause_phase) {
      phase += delta_seconds() * 60.0f;
      if (phase > 360.0f) phase -= 360.0f;
    }

    imgui.render([] { ImGui::ShowDemoWindow(); });
  }
};

int main() {
  Colors application;
  application.run();
}