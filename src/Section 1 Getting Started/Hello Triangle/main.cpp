#include <iphelf/opengl/application.h>

#include <filesystem>

const auto path_shaders = std::filesystem::current_path() / "shaders";

// Each vertex has one attribute (position: vec2)
const auto path_vs_position = path_shaders / "position.v.glsl";
const auto path_fs_uniform = path_shaders / "uniform.f.glsl";
const auto path_fs_position = path_shaders / "position.f.glsl";

// Each vertex has two attributes (position: vec2, color: vec3)
const auto path_vs_position_color = path_shaders / "position_color.v.glsl";
const auto path_fs_color = path_shaders / "color.f.glsl";

class HelloTriangle : public iphelf::opengl::Application {
 public:
  HelloTriangle() : iphelf::opengl::Application(800, 600, "Hello, Triangle") {
    background_color = iphelf::opengl::Color::DarkGreenBluish;

    /*
     * First quarter: 1 white triangle, 1 black triangle
     * Second quarter: 2 orange triangles (1 rectangle)
     * Center: 1 position-colored triangle
     * Third quarter: 1 RGB triangle
     * Fourth quarter: 1 CMY triangle
     */

    // First quarter
    triangles_q1_white = create_triangle_array<2>({
        {{0.5f, 0.25f}, {0.75f, 0.25f}, {0.75f, 0.75f}},
    });
    triangles_q1_black = create_triangle_array<2>({
        {{0.25f, 0.5f}, {0.25f, 0.75f}, {0.75f, 0.75f}},
    });
    program_uniform_color = create_program(path_vs_position, path_fs_uniform);

    // Second quarter
    triangles_q2_orange = create_triangle_array<2>(
        {{-0.75f, 0.5f}, {-0.25f, 0.5f}, {-0.25f, 0.75f}, {-0.75f, 0.75f}},
        {{0, 1, 2}, {0, 2, 3}});

    // Center
    triangles_c_position = create_triangle_array<2>({
        {{-0.3f, -0.3f}, {0.3f, -0.3f}, {0.0f, 0.3f}},
    });
    program_position_as_color =
        create_program(path_vs_position, path_fs_position);

    // Third quarter
    triangles_q3_rgb = create_triangle_array<5>(
        {
            {{-0.75f, -0.75f, 0.0f, 0.0f, 1.0f},
             {-0.25f, -0.75f, 1.0f, 0.0f, 0.0f},
             {-0.75f, -0.25f, 0.0f, 1.0f, 0.0f}},
        },
        {2, 3});
    program_with_color = create_program(path_vs_position_color, path_fs_color);

    // Fourth quarter
    triangles_q4_cmy = create_triangle_array<5>(
        {
            {{0.25f, -0.75f, 1.0f, 0.0f, 1.0f},
             {0.75f, -0.75f, 0.0f, 1.0f, 1.0f},
             {0.75f, -0.25f, 1.0f, 1.0f, 0.0f}},
        },
        {2, 3});
  }

 private:
  void render() override {
    clear(background_color);

    program_uniform_color.with_uniform("ourColor",
                                       iphelf::opengl::Color::White);
    program_uniform_color.render_wireframe(triangles_q1_white);
    program_uniform_color.with_uniform("ourColor",
                                       iphelf::opengl::Color::Black);
    program_uniform_color.render(triangles_q1_black);
    program_uniform_color.with_uniform("ourColor",
                                       iphelf::opengl::Color::Orange);
    program_uniform_color.render(triangles_q2_orange);

    program_position_as_color.render(triangles_c_position);

    program_with_color.render(triangles_q3_rgb);
    program_with_color.render(triangles_q4_cmy);
  }

  iphelf::opengl::Color background_color;

  iphelf::opengl::TriangleArray triangles_q1_white;
  iphelf::opengl::TriangleArray triangles_q1_black;
  iphelf::opengl::TriangleArray triangles_q2_orange;
  iphelf::opengl::Program program_uniform_color;

  iphelf::opengl::TriangleArray triangles_c_position;
  iphelf::opengl::Program program_position_as_color;

  iphelf::opengl::TriangleArray triangles_q3_rgb;
  iphelf::opengl::TriangleArray triangles_q4_cmy;
  iphelf::opengl::Program program_with_color;
};

int main() {
  HelloTriangle application;
  application.run();
}
