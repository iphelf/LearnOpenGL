#include <iphelf/opengl/application.h>

#include <filesystem>

const auto path_shaders = std::filesystem::current_path() / "shaders";

// Each vertex has two attribute (pos: vec2, uv: vec2)
const auto path_vs_pos_uv = path_shaders / "pos_uv.v.glsl";
const auto path_fs_image = path_shaders / "image.f.glsl";

const auto path_textures = std::filesystem::current_path() / "textures";

const auto path_image_wall = path_textures / "wall.jpg";

class Textures : public iphelf::opengl::Application {
  iphelf::opengl::Color background_color;
  iphelf::opengl::TriangleArray triangles_wall;
  iphelf::opengl::Texture image_wall;
  iphelf::opengl::Program program_image;

 public:
  Textures() : iphelf::opengl::Application(800, 600, "Textures") {
    background_color = iphelf::opengl::Color::DarkGreenBluish;

    triangles_wall = create_triangle_array<4>(
        {
            {-0.5, -0.5, 0, 0},
            {0.5, -0.5, 1, 0},
            {0.5, 0.5, 1, 1},
            {-0.5, 0.5, 0, 1},
        },
        {
            {0, 1, 2},
            {0, 2, 3},
        },
        {2, 2});
    image_wall = create_texture(path_image_wall);
    program_image = create_program(path_vs_pos_uv, path_fs_image);
  }

 private:
  void render() override {
    clear(background_color);
    program_image.bind_texture(image_wall);
    program_image.render(triangles_wall);
  }
};

int main() {
  Textures application;
  application.run();
}
