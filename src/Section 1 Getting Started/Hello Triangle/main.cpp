#include <iphelf/opengl/application.h>

#include <filesystem>

const auto path_shaders = std::filesystem::current_path() / "shaders";

// Each vertex has one attribute (position: vec2)
const auto path_vs_position = path_shaders / "position.v.glsl";
const auto path_fs_orange = path_shaders / "orange.f.glsl";
const auto path_fs_white = path_shaders / "white.f.glsl";

class HelloTriangle : public iphelf::opengl::Application {
  iphelf::opengl::Color background_color;
  iphelf::opengl::TriangleArray triangles_a;
  iphelf::opengl::TriangleArray triangles_b;
  iphelf::opengl::Program program_orange;
  iphelf::opengl::Program program_white;

 public:
  HelloTriangle() : iphelf::opengl::Application(800, 600, "Hello, Triangle") {
    background_color = iphelf::opengl::Colors::DarkGreenBluish;
    triangles_a = create_triangle_array<2>(
        {
            {0.5f, 0.5f},    // top right
            {0.5f, -0.5f},   // bottom right
            {-0.5f, -0.5f},  // bottom left
            {-0.5f, 0.5f},   // top left
        },
        {{0, 1, 3}, {1, 2, 3}});
    program_orange = create_program(path_vs_position, path_fs_orange);
    triangles_b = create_triangle_array<2>({{
        {0.0f, 0.3f},    // top
        {-0.3f, -0.2f},  // bottom left
        {0.3f, -0.2f},   // bottom right
    }});
    program_white = create_program(path_vs_position, path_fs_white);
  }

 private:
  void render() override {
    clear(background_color);
    program_orange.render_wireframe(triangles_a);
    program_white.render(triangles_b);
  }
};

int main() {
  HelloTriangle application;
  application.run();
}
