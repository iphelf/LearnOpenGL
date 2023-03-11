#include <iphelf/imgui/instance.h>
#include <iphelf/opengl/application.h>
#include <rapidcsv.h>

#include <functional>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

const auto path_shaders{std::filesystem::current_path() / "shaders"};

class Materials : public iphelf::opengl::Application {
  iphelf::opengl::Camera camera{
      create_camera(glm::vec3{2.0f, -2.0f, 2.0f},
                    {{0.0f, 0.0f, 1.0f}, {-1.0f, 1.0f, -1.0f}, 0.0f, -20.0f})};
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
  struct Material {
    std::string name;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;
  };
  const std::vector<Material> materials{std::invoke([] {
    // Source: http://devernay.free.fr/cours/opengl/materials.html
    const auto path_file{std::filesystem::current_path() /
                         "opengl_vrml_materials.txt"};
    rapidcsv::Document doc{path_file.string(), rapidcsv::LabelParams(0, 0)};
    std::vector<Material> ms;
    ms.reserve(doc.GetRowCount());
    for (auto name : doc.GetRowNames()) {
      auto fields{doc.GetRow<float>(name)};
      ms.push_back({.name = std::move(name),
                    .ambient = {fields[0], fields[1], fields[2]},
                    .diffuse = {fields[3], fields[4], fields[5]},
                    .specular = {fields[6], fields[7], fields[8]},
                    .shininess = fields[9] * 128.0f});
    }
    return ms;
  })};
  iphelf::imgui::Instance imgui{get_glfw_window()};

 public:
  Materials() : Application(800, 600, "Materials") {
    enable_depth_test();
    bind_default_camera_controller(camera);
    imgui.install_callbacks();
    glm::mat3 object_model2world_normalized{
        glm::transpose(glm::inverse(object_model2world))};
    program_object.with_uniform("u_model2world", object_model2world);
    program_object.with_uniform("u_model2world_normalized",
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

    program_object.with_uniform("u_camera_pos", camera.position());
    program_object.with_uniform("u_light.position",
                                glm::vec3{rotated_light_model2world *
                                          glm::vec4{glm::vec3{0.0f}, 1.0f}});
    program_object.with_uniform("u_light.ambient", light_color);
    program_object.with_uniform("u_light.diffuse", light_color);
    program_object.with_uniform("u_light.specular", light_color);
    static std::size_t selected_material{0};
    {
      auto &material = materials[selected_material];
      program_object.with_uniform("u_material.ambient", material.ambient);
      program_object.with_uniform("u_material.diffuse", material.diffuse);
      program_object.with_uniform("u_material.specular", material.specular);
      program_object.with_uniform("u_material.shininess", material.shininess);
    }
    program_object.with_uniform("u_world2view", world2view);
    program_object.with_uniform("u_view2clip", view2clip);

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

    imgui.render([this] {
      ImGui::Checkbox("pause light rotation", &pause_phase_rotation);
      ImGui::Checkbox("pause light hue update", &pause_phase_hue);
      ImGui::Checkbox("specify light color", &specify_light_color);
      ImGui::BeginDisabled(!specify_light_color);
      { ImGui::ColorEdit3("light color", &specified_light_color.r); }
      ImGui::EndDisabled();

      if (ImGui::BeginCombo("material",
                            materials[selected_material].name.data())) {
        for (std::size_t i{0}; i < materials.size(); ++i) {
          bool selected{i == selected_material};
          if (ImGui::Selectable(materials[i].name.data(), selected))
            selected_material = i;
          if (selected) ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
      }

      static bool show_demo_window{false};
      ImGui::Checkbox("show demo window", &show_demo_window);
      if (show_demo_window) ImGui::ShowDemoWindow();
    });
  }
};

int main() {
  Materials application;
  application.run();
}