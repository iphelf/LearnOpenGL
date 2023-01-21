#include <iphelf/opengl/application.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

const auto path_shaders = std::filesystem::current_path() / "shaders";
const auto path_vs = path_shaders / "pos_uv.v.glsl";
const auto path_fs = path_shaders / "image.f.glsl";

const auto path_textures = std::filesystem::current_path() / "textures";
const auto path_image_face = path_textures / "awesomeface.png";

class Transformations : public iphelf::opengl::Application {
  iphelf::opengl::Program program;
  iphelf::opengl::TriangleArray object;
  iphelf::opengl::Texture texture;
  const glm::mat4 identity{1.0f};

 public:
  Transformations() : Application(800, 800, "Transformations") {
    program = create_program(path_vs, path_fs);
    object = create_triangle_array<4>(
        {
            {-1.0f, -1.0f, 0.0f, 0.0f},
            {1.0f, -1.0f, 1.0f, 0.0f},
            {1.0f, 1.0f, 1.0f, 1.0f},
            {-1.0f, 1.0f, 0.0f, 1.0f},
        },
        {
            {0, 1, 2},
            {0, 2, 3},
        },
        {2, 2});
    texture = create_texture(path_image_face);
  }

 private:
  void render() override {
    clear(iphelf::opengl::Color::Orange);

    program.bind_texture(texture);

    static const auto scaling{glm::scale(identity, glm::vec3{0.5f})};
    static const auto translation_tl{glm::translate(identity, {-0.5, 0.5, 0})};
    static const auto translation_br{glm::translate(identity, {0.5, -0.5, 0})};
    static const auto transform_tl{translation_tl * scaling};
    static const auto transform_br{translation_br * scaling};
    auto seconds{elapsed_seconds()};

    auto rotation_over_time{
        glm::rotate(identity, glm::radians(seconds * 60.0f), {0, 0, 1})};
    program.with_uniform("u_transform", transform_br * rotation_over_time);
    program.render(object);

    auto scaling_over_time{glm::scale(identity, glm::vec3{glm::sin(seconds)})};
    program.with_uniform("u_transform", transform_tl * scaling_over_time);
    program.render(object);
  }
};

int main() {
  Transformations application;
  application.run();
}