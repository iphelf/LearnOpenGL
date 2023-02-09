#include <iphelf/opengl/application.h>

#include <filesystem>
#include <functional>
#include <iostream>

const auto path_shaders = std::filesystem::current_path() / "shaders";

// Each vertex has two attributes (pos: vec2, uv: vec2)
const auto path_vs_pos_uv = path_shaders / "xy_uv.v.glsl";
const auto path_fs_image = path_shaders / "image.f.glsl";

// Each vertex has three attributes (pos: vec2, uv: vec2, color: vec3)
const auto path_vs_pos_uv_color = path_shaders / "xy_uv_rgb.v.glsl";
const auto path_fs_blend = path_shaders / "blend.f.glsl";

const auto path_textures = std::filesystem::current_path() / "textures";

const auto path_image_wall = path_textures / "wall.jpg";
const auto path_image_face = path_textures / "awesomeface.png";
const auto path_image_container = path_textures / "container.jpg";

class Textures : public iphelf::opengl::Application {
  iphelf::opengl::Color background_color;

  iphelf::opengl::Program program_image;

  iphelf::opengl::TriangleArray triangles_background;
  iphelf::opengl::Texture image_wall_tiled;

  iphelf::opengl::TriangleArray triangles_mirrored_faces;
  iphelf::opengl::Texture image_face_mirrored;

  iphelf::opengl::TriangleArray triangles_bordered_faces;
  iphelf::opengl::Texture image_face_bordered;

  iphelf::opengl::TriangleArray triangles_edged_container;
  iphelf::opengl::Texture image_container_edged;

  iphelf::opengl::Program program_blending;
  iphelf::opengl::Texture image_face_edged_linear;
  iphelf::opengl::Texture image_face_edged_nearest;
  iphelf::opengl::TriangleArray triangles_blending;
  float image_scale{1.0f};
  const float scale_asc_step{1.25f};
  const float scale_des_step{0.8f};
  float offset_u{0.0f};
  float offset_v{0.0f};
  const float offset_step{0.1f};
  bool use_linear{false};
  float intensity{0.0f};

 public:
  Textures() : iphelf::opengl::Application(800, 600, "Textures") {
    background_color = iphelf::opengl::Colors::DarkGreenBluish;

    program_image = create_program(path_vs_pos_uv, path_fs_image);

    // tiled wall as the background
    triangles_background = create_triangle_array<4>(
        {
            {-1.0f, -1.0f, 0.0f, 0.0f},
            {1.0f, -1.0f, 3.0f, 0.0f},
            {1.0f, 1.0f, 3.0f, 3.0f},
            {-1.0f, 1.0f, 0.0f, 3.0f},
        },
        {
            {0, 1, 2},
            {0, 2, 3},
        },
        {2, 2});
    image_wall_tiled = create_texture(
        path_image_wall, iphelf::opengl::Texture::WrappingMode::Repeat);

    // mirrored faces in the second quarter
    triangles_mirrored_faces = create_triangle_array<4>(
        {
            {-0.85f, 0.15f, 0.0f, 0.0f},
            {-0.15f, 0.15f, 2.0f, 0.0f},
            {-0.15f, 0.85f, 2.0f, 2.0f},
            {-0.85f, 0.85f, 0.0f, 2.0f},
        },
        {
            {0, 1, 2},
            {0, 2, 3},
        },
        {2, 2});
    image_face_mirrored = create_texture(
        path_image_face, iphelf::opengl::Texture::WrappingMode::MirroredRepeat);

    // bordered face in the first quarter
    triangles_bordered_faces = create_triangle_array<4>(
        {
            {0.15f, 0.15f, -0.5f, -0.5f},
            {0.85f, 0.15f, 1.5f, -0.5f},
            {0.85f, 0.85f, 1.5f, 1.5f},
            {0.15f, 0.85f, -0.5f, 1.5f},
        },
        {
            {0, 1, 2},
            {0, 2, 3},
        },
        {2, 2});
    image_face_bordered = create_texture(
        path_image_face, iphelf::opengl::Texture::WrappingMode::ClampToBorder,
        iphelf::opengl::Colors::Yellow);

    // edged container in the third quarter
    triangles_edged_container = create_triangle_array<4>(
        {
            {-0.85f, -0.85f, -0.5f, -0.5f},
            {-0.15f, -0.85f, 1.5f, -0.5f},
            {-0.15f, -0.15f, 1.5f, 1.5f},
            {-0.85f, -0.15f, -0.5f, 1.5f},
        },
        {
            {0, 1, 2},
            {0, 2, 3},
        },
        {2, 2});
    image_container_edged =
        create_texture(path_image_container,
                       iphelf::opengl::Texture::WrappingMode::ClampToEdge);

    // blending images in the fourth quarter
    program_blending = create_program(path_vs_pos_uv_color, path_fs_blend);
    image_face_edged_linear = create_texture(
        path_image_face, iphelf::opengl::Texture::WrappingMode::ClampToEdge,
        iphelf::opengl::Texture::FilterType::Linear);
    image_face_edged_nearest = create_texture(
        path_image_face, iphelf::opengl::Texture::WrappingMode::ClampToEdge,
        iphelf::opengl::Texture::FilterType::Nearest);
    triangles_blending = create_triangle_array<7>(
        {
            {0.15f, -0.85f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f},
            {0.85f, -0.85f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f},
            {0.85f, -0.15f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f},
            {0.15f, -0.15f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f},
        },
        {
            {0, 1, 2},
            {0, 2, 3},
        },
        {2, 2, 3});
  }

 private:
  void handle_keys() {
    // Move around using -, +, up, down, left, right
    if (just_released(iphelf::opengl::Key::MinusUnderscore)) {
      image_scale *= scale_asc_step;
      std::cout << "Zoomed out a bit\n" << std::flush;
    }
    if (just_released(iphelf::opengl::Key::EqualPlus)) {
      image_scale *= scale_des_step;
      std::cout << "Zoomed in a bit\n" << std::flush;
    }
    if (just_released(iphelf::opengl::Key::Up)) {
      offset_v += offset_step;
      std::cout << "Panned upward a bit\n" << std::flush;
    }
    if (just_released(iphelf::opengl::Key::Down)) {
      offset_v -= offset_step;
      std::cout << "Panned downward a bit\n" << std::flush;
    }
    if (just_released(iphelf::opengl::Key::Left)) {
      offset_u -= offset_step;
      std::cout << "Panned to the left a bit\n" << std::flush;
    }
    if (just_released(iphelf::opengl::Key::Right)) {
      offset_u += offset_step;
      std::cout << "Panned to the right a bit\n" << std::flush;
    }

    // Switch filter using TAB
    if (just_released(iphelf::opengl::Key::Tab)) {
      use_linear = !use_linear;
      std::cout << "Switch to " << (use_linear ? "linear" : "nearest")
                << "filter\n"
                << std::flush;
    }

    // Change intensity of gradient color using 0~9
    intensity = std::invoke([this]() {
      if (just_released(iphelf::opengl::Key::_0)) return 0.0f / 9.0f;
      if (just_released(iphelf::opengl::Key::_1)) return 1.0f / 9.0f;
      if (just_released(iphelf::opengl::Key::_2)) return 2.0f / 9.0f;
      if (just_released(iphelf::opengl::Key::_3)) return 3.0f / 9.0f;
      if (just_released(iphelf::opengl::Key::_4)) return 4.0f / 9.0f;
      if (just_released(iphelf::opengl::Key::_5)) return 5.0f / 9.0f;
      if (just_released(iphelf::opengl::Key::_6)) return 6.0f / 9.0f;
      if (just_released(iphelf::opengl::Key::_7)) return 7.0f / 9.0f;
      if (just_released(iphelf::opengl::Key::_8)) return 8.0f / 9.0f;
      if (just_released(iphelf::opengl::Key::_9)) return 9.0f / 9.0f;
      return intensity;
    });
  }

  void render() override {
    handle_keys();

    clear(background_color);

    program_image.bind_texture(image_wall_tiled);
    program_image.render(triangles_background);

    program_image.bind_texture(image_face_mirrored);
    program_image.render(triangles_mirrored_faces);

    program_image.bind_texture(image_face_bordered);
    program_image.render(triangles_bordered_faces);

    program_image.bind_texture(image_container_edged);
    program_image.render(triangles_edged_container);

    program_blending.bind_texture("u_image_container", 0,
                                  image_container_edged);
    program_blending.bind_texture(
        "u_image_face", 1,
        use_linear ? image_face_edged_linear : image_face_edged_nearest);
    program_blending.with_uniform("u_scale", image_scale);
    program_blending.with_uniform("u_offset_u", offset_u);
    program_blending.with_uniform("u_offset_v", offset_v);
    program_blending.with_uniform("u_intensity", intensity);
    program_blending.render(triangles_blending);
  }
};

int main() {
  Textures application;
  application.run();
}
