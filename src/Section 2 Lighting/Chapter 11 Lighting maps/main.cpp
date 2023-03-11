#include <iphelf/imgui/instance.h>
#include <iphelf/opengl/application.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

const auto path_shaders{std::filesystem::current_path() / "shaders"};
const auto path_textures{std::filesystem::current_path() / "textures"};

class LightingMaps : public iphelf::opengl::Application {
  const iphelf::opengl::Program program_light{create_program(
      path_shaders / "light.v.glsl", path_shaders / "light.f.glsl")};
  const iphelf::opengl::Program program_object{create_program(
      path_shaders / "object.v.glsl", path_shaders / "object.f.glsl")};
  const iphelf::opengl::Texture texture_image{
      create_texture(path_textures / "container2.png")};
  const iphelf::opengl::Texture texture_specular{
      create_texture(path_textures / "container2_specular.png")};
  const iphelf::opengl::Texture texture_cover{
      create_texture(path_textures / "matrix.jpg",
                     iphelf::opengl::Texture::WrappingMode::ClampToBorder,
                     iphelf::opengl::Colors::Black)};
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
  iphelf::opengl::Camera camera{
      create_camera(glm::vec3{2.0f, -2.0f, 2.0f},
                    {{0.0f, 0.0f, 1.0f}, {-1.0f, 1.0f, -1.0f}, 0.0f, -20.0f})};
  const glm::mat4 light_model2world{std::invoke([] {
    glm::mat4 model2world{1.0f};
    model2world = glm::translate(model2world, glm::vec3{0.0f, 2.0f, 2.0f});
    model2world = glm::scale(model2world, glm::vec3{0.1f});
    return model2world;
  })};
  iphelf::imgui::Instance imgui{get_glfw_window()};

 public:
  LightingMaps() : Application(800, 600, "Lighting maps") {
    enable_depth_test();
    bind_default_camera_controller(camera);
    imgui.install_callbacks();

    program_object.with_uniform("u_model2world", glm::mat4{1.0f});
    program_object.with_uniform("u_model2world_normalized",
                                glm::transpose(glm::inverse(glm::mat3{1.0f})));
  }

 private:
  void render() override {
    clear(iphelf::opengl::Colors::DarkGreenBluish);

    auto view2clip{camera.view2clip()};
    auto world2view{camera.world2view()};

    static float phase_hue{0.0f};
    static bool specify_light_color{true};
    static auto specified_light_color{iphelf::opengl::Colors::White};
    auto light_color{specify_light_color ? specified_light_color
                                         : iphelf::opengl::Color::from_hsv(
                                               phase_hue, 1.0f, 1.0f)};
    static float phase_rotation{0.0f};
    auto rotated_light_model2world{glm::rotate(glm::mat4{1.0f},
                                               glm::radians(phase_rotation),
                                               glm::vec3{0.0f, 0.0f, 1.0f}) *
                                   light_model2world};
    program_light.with_uniform("u_model2world", rotated_light_model2world);
    program_light.with_uniform("u_world2view", world2view);
    program_light.with_uniform("u_view2clip", view2clip);
    program_light.with_uniform("u_light_color", light_color);
    program_light.render(cube);

    program_object.with_uniform("u_world2view", world2view);
    program_object.with_uniform("u_view2clip", view2clip);
    program_object.with_uniform("u_camera_pos", camera.position());
    auto light_position{glm::vec3{rotated_light_model2world *
                                  glm::vec4{glm::vec3{0.0f}, 1.0f}}};
    program_object.with_uniform("u_light.position", light_position);
    static float ambient_intensity{0.1f};
    static float diffuse_intensity{1.0f};
    static float specular_intensity{0.5f};
    program_object.with_uniform("u_light.ambient",
                                light_color * ambient_intensity);
    program_object.with_uniform("u_light.diffuse",
                                light_color * diffuse_intensity);
    program_object.with_uniform("u_light.specular",
                                light_color * specular_intensity);
    program_object.bind_texture("u_material.emission", 0, texture_cover);
    static float emission_intensity{1.0f};
    program_object.with_uniform("u_material.emission_intensity",
                                emission_intensity);
    program_object.bind_texture("u_material.diffuse", 1, texture_image);
    program_object.bind_texture("u_material.specular", 2, texture_specular);
    static float shininess{32.0f};
    program_object.with_uniform("u_material.shininess", shininess);
    program_object.render(cube);

    static bool pause_phase_hue{true};
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
      ImGui::Checkbox("pause light rotation", &pause_phase_rotation);
      ImGui::Checkbox("pause light hue update", &pause_phase_hue);
      ImGui::Checkbox("specify light color", &specify_light_color);
      ImGui::BeginDisabled(!specify_light_color);
      { ImGui::ColorEdit3("light color", &specified_light_color.r); }
      ImGui::EndDisabled();
      // ambient / diffuse / specular
      ImGui::SliderFloat("ambient light intensity", &ambient_intensity, 0.0f,
                         1.0f, "%.2f");
      ImGui::SliderFloat("diffuse light intensity", &diffuse_intensity, 0.0f,
                         1.0f, "%.2f");
      ImGui::SliderFloat("specular light intensity", &specular_intensity, 0.0f,
                         1.0f, "%.2f");

      ImGui::SliderFloat("object emission intensity", &emission_intensity, 0.0f,
                         1.0f, "%.2f");
      ImGui::SliderFloat("object shininess", &shininess, 1.0f, 256.0f, "%.0f",
                         ImGuiSliderFlags_Logarithmic);

      static bool show_demo_window{false};
      ImGui::Checkbox("show demo window", &show_demo_window);
      if (show_demo_window) ImGui::ShowDemoWindow();
    });
  }
};

int main() {
  LightingMaps application;
  application.run();
}