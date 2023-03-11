#include <iphelf/imgui/instance.h>
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
  const iphelf::opengl::Program program_object_phong{
      create_program(path_shaders / "object_phong.v.glsl",
                     path_shaders / "object_phong.f.glsl")};
  const iphelf::opengl::Program program_object_gouraud{
      create_program(path_shaders / "object_gouraud.v.glsl",
                     path_shaders / "object_gouraud.f.glsl")};
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
  const iphelf::opengl::Color object_color{iphelf::opengl::Colors::Orange};
  iphelf::imgui::Instance imgui{get_glfw_window()};

 public:
  Colors() : Application(800, 600, "Basic Lighting") {
    enable_depth_test();
    bind_default_camera_controller(camera);
    imgui.install_callbacks();
    glm::mat3 object_model2world_normalized{
        glm::transpose(glm::inverse(object_model2world))};
    program_object_phong.with_uniform("u_object_color", object_color);
    program_object_phong.with_uniform("u_model2world", object_model2world);
    program_object_phong.with_uniform("u_model2world_normalized",
                                      object_model2world_normalized);
    program_object_gouraud.with_uniform("u_object_color", object_color);
    program_object_gouraud.with_uniform("u_model2world", object_model2world);
    program_object_gouraud.with_uniform("u_model2world_normalized",
                                        object_model2world_normalized);
  }

 private:
  void render() override {
    clear(iphelf::opengl::Colors::Black);

    auto view2clip{camera.view2clip()};
    auto world2view{camera.world2view()};
    static float phase_hue{0.0f};
    static bool specify_light_color{false};
    static iphelf::opengl::Color specified_light_color;
    auto light_color{specify_light_color ? specified_light_color
                                         : iphelf::opengl::Color::from_hsv(
                                               phase_hue, 1.0f, 1.0f)};
    static float phase_rotation{0.0f};
    auto rotated_light_model2world{glm::rotate(glm::mat4{1.0f},
                                               glm::radians(phase_rotation),
                                               glm::vec3{0.0f, 0.0f, 1.0f}) *
                                   light_model2world};

    program_light.with_uniform("u_light_color", light_color);
    program_light.with_uniform("u_model2world", rotated_light_model2world);
    program_light.with_uniform("u_world2view", world2view);
    program_light.with_uniform("u_view2clip", view2clip);
    program_light.render(cube);

    static bool use_phong{true};
    auto &program_object =
        use_phong ? program_object_phong : program_object_gouraud;
    program_object.with_uniform("u_light_pos",
                                glm::vec3{rotated_light_model2world *
                                          glm::vec4{glm::vec3{0.0f}, 1.0f}});
    program_object.with_uniform("u_camera_pos", camera.position());
    program_object.with_uniform("u_light_color", light_color);
    program_object.with_uniform("u_world2view", world2view);
    program_object.with_uniform("u_view2clip", view2clip);
    static float ambient_strength{0.1f};
    static float diffuse_strength{1.0f};
    static float specular_strength{0.5f};
    static float shininess{32.0f};
    program_object.with_uniform("u_ambient_strength", ambient_strength);
    program_object.with_uniform("u_diffuse_strength", diffuse_strength);
    program_object.with_uniform("u_specular_strength", specular_strength);
    program_object.with_uniform("u_shininess", shininess);
    program_object.render(cube);

    static bool pause_phase_hue{false};
    static bool pause_phase_rotation{false};
    {
      auto increment_phase{[](float &phase, float delta) {
        phase += delta;
        if (phase > 360.0f) phase -= 360.0f;
      }};
      float delta{delta_seconds() * 60.0f};
      if (!pause_phase_hue) increment_phase(phase_hue, delta);
      if (!pause_phase_rotation) increment_phase(phase_rotation, delta);
    }

    imgui.render([] {
      {
        if (ImGui::RadioButton("use phong shading", use_phong))
          use_phong = true;
        if (ImGui::RadioButton("use gouraud shading", !use_phong))
          use_phong = false;
      }
      ImGui::Checkbox("pause light rotation", &pause_phase_rotation);
      ImGui::Checkbox("pause light hue update", &pause_phase_hue);
      ImGui::Checkbox("specify light color", &specify_light_color);
      ImGui::BeginDisabled(!specify_light_color);
      { ImGui::ColorEdit3("light color", &specified_light_color.r); }
      ImGui::EndDisabled();
      ImGui::SliderFloat("ambient strength", &ambient_strength, 0.0f, 1.0f,
                         "%.2f");
      ImGui::SliderFloat("diffuse strength", &diffuse_strength, 0.0f, 1.0f,
                         "%.2f");
      ImGui::SliderFloat("specular strength", &specular_strength, 0.0f, 1.0f,
                         "%.2f");
      ImGui::SliderFloat("shininess", &shininess, 1.0f, 256.0f, "%.0f",
                         ImGuiSliderFlags_Logarithmic);
      static bool show_demo_window{false};
      ImGui::Checkbox("show demo window", &show_demo_window);
      if (show_demo_window) ImGui::ShowDemoWindow();
    });
  }
};

int main() {
  Colors application;
  application.run();
}