#include <iphelf/opengl/application.h>

#include <fstream>
#include <functional>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <numeric>

const auto path_shaders = std::filesystem::current_path() / "shaders";
const auto path_vs = path_shaders / "transform_uv.v.glsl";
const auto path_fs = path_shaders / "image.f.glsl";

const auto path_textures = std::filesystem::current_path() / "textures";
const auto path_image_face = path_textures / "awesomeface.png";
const auto path_image_container = path_textures / "container.jpg";
const auto path_image_wall = path_textures / "wall.jpg";

const auto path_maze = std::filesystem::current_path() / "maze.txt";

class CoordinateSystems : public iphelf::opengl::Application {
  struct Level {
    // (row, column) / (z, x)
    std::pair<int, int> size{1, 1};
    // (row, column) / (z, x)
    std::pair<int, int> player_start{0, 0};
    std::vector<std::vector<bool>> is_block{};
    explicit Level(const std::filesystem::path &path) {
      // load level row by row
      {
        std::ifstream fin{path};
        std::string line;
        int i = 0;
        bool player_found = false;
        while (std::getline(fin, line) && !line.empty()) {
          if (!player_found) {
            if (auto found{line.find('O')}; found != std::string::npos) {
              player_start.first = i;
              player_start.second = static_cast<int>(found);
              player_found = true;
            }
          }
          is_block.push_back(std::invoke([&line] {
            std::vector<bool> row(line.size());
            std::transform(line.begin(), line.end(), row.begin(),
                           [](char c) { return c == '#'; });
            return row;
          }));
          size.second = std::max(size.second, static_cast<int>(line.size()));
          ++i;
        }
      }
      // pad row-ends
      for (auto &row : is_block) {
        if (std::ssize(row) < size.second)
          row.insert(row.end(), size.second - row.size(), false);
      }
      size.first = static_cast<int>(is_block.size());
    }
  } const level{path_maze};
  std::pair<int, int> player_location{level.player_start};

  glm::mat4 translate_on_gird(int i, int j) {
    return glm::translate(identity, {(1 - level.size.second) * 0.5 + j, 0.5,
                                     (1 - level.size.first) * 0.5 + i});
  }

  const iphelf::opengl::TriangleArray plane{
      std::invoke([max_u = static_cast<float>(level.size.second),
                   max_v = static_cast<float>(level.size.first)] {
        return create_triangle_array<5>(
            {
                {-0.5f, 0.0f, 0.5f, 0.0f, 0.0f},
                {0.5f, 0.0f, 0.5f, max_u, 0.0f},
                {0.5f, 0.0f, -0.5f, max_u, max_v},
                {-0.5f, 0.0f, -0.5f, 0.0f, max_v},
            },
            {
                {0, 1, 2},
                {0, 2, 3},
            },
            {3, 2});
      })};
  const iphelf::opengl::TriangleArray cube{std::invoke([] {
    return create_triangle_array<5>(
        {
            {{-0.5f, -0.5f, -0.5f, 0.0f, 0.0f},
             {0.5f, -0.5f, -0.5f, 1.0f, 0.0f},
             {0.5f, 0.5f, -0.5f, 1.0f, 1.0f}},
            {{0.5f, 0.5f, -0.5f, 1.0f, 1.0f},
             {-0.5f, 0.5f, -0.5f, 0.0f, 1.0f},
             {-0.5f, -0.5f, -0.5f, 0.0f, 0.0f}},
            {{-0.5f, -0.5f, 0.5f, 0.0f, 0.0f},
             {0.5f, -0.5f, 0.5f, 1.0f, 0.0f},
             {0.5f, 0.5f, 0.5f, 1.0f, 1.0f}},
            {{0.5f, 0.5f, 0.5f, 1.0f, 1.0f},
             {-0.5f, 0.5f, 0.5f, 0.0f, 1.0f},
             {-0.5f, -0.5f, 0.5f, 0.0f, 0.0f}},
            {{-0.5f, 0.5f, 0.5f, 1.0f, 0.0f},
             {-0.5f, 0.5f, -0.5f, 1.0f, 1.0f},
             {-0.5f, -0.5f, -0.5f, 0.0f, 1.0f}},
            {{-0.5f, -0.5f, -0.5f, 0.0f, 1.0f},
             {-0.5f, -0.5f, 0.5f, 0.0f, 0.0f},
             {-0.5f, 0.5f, 0.5f, 1.0f, 0.0f}},
            {{0.5f, 0.5f, 0.5f, 1.0f, 0.0f},
             {0.5f, 0.5f, -0.5f, 1.0f, 1.0f},
             {0.5f, -0.5f, -0.5f, 0.0f, 1.0f}},
            {{0.5f, -0.5f, -0.5f, 0.0f, 1.0f},
             {0.5f, -0.5f, 0.5f, 0.0f, 0.0f},
             {0.5f, 0.5f, 0.5f, 1.0f, 0.0f}},
            {{-0.5f, -0.5f, -0.5f, 0.0f, 1.0f},
             {0.5f, -0.5f, -0.5f, 1.0f, 1.0f},
             {0.5f, -0.5f, 0.5f, 1.0f, 0.0f}},
            {{0.5f, -0.5f, 0.5f, 1.0f, 0.0f},
             {-0.5f, -0.5f, 0.5f, 0.0f, 0.0f},
             {-0.5f, -0.5f, -0.5f, 0.0f, 1.0f}},
            {{-0.5f, 0.5f, -0.5f, 0.0f, 1.0f},
             {0.5f, 0.5f, -0.5f, 1.0f, 1.0f},
             {0.5f, 0.5f, 0.5f, 1.0f, 0.0f}},
            {{0.5f, 0.5f, 0.5f, 1.0f, 0.0f},
             {-0.5f, 0.5f, 0.5f, 0.0f, 0.0f},
             {-0.5f, 0.5f, -0.5f, 0.0f, 1.0f}},
        },
        {3, 2});
  })};

  const iphelf::opengl::Texture image_wall{create_texture(
      path_image_wall, iphelf::opengl::Texture::WrappingMode::Repeat)};
  const iphelf::opengl::Texture image_container{
      create_texture(path_image_container)};
  const iphelf::opengl::Texture image_face{create_texture(path_image_face)};

  const iphelf::opengl::Program program{create_program(path_vs, path_fs)};

  const glm::mat4 identity{1.0f};

  // model-to-world matrices
  const glm::mat4 floor_plane{
      glm::scale(identity, {level.size.second, 1, level.size.first})};
  const std::vector<glm::mat4> block_cubes{std::invoke([&level = level,
                                                        identity = identity] {
    std::vector<glm::mat4> blocks;
    blocks.reserve(std::accumulate(
        level.is_block.begin(), level.is_block.end(), 0,
        [](int acc, const auto &row) {
          return acc + static_cast<int>(std::count_if(row.begin(), row.end(),
                                                      std::identity{}));
        }));
    glm::vec3 base{(1 - level.size.second) * 0.5, 0.5f,
                   (1 - level.size.first) * 0.5};
    for (int i{0}; i < level.size.first; ++i)
      for (int j{0}; j < level.size.second; ++j)
        if (level.is_block[i][j])
          blocks.push_back(glm::translate(identity, base + glm::vec3{j, 0, i}));
    return blocks;
  })};
  glm::mat4 player_cube{
      translate_on_gird(player_location.first, player_location.second)};

  // world-to-view matrix (camera)
  glm::mat4 world2view{std::invoke([identity = identity] {
    glm::mat4 view{identity};
    view = glm::rotate(identity, glm::radians(55.0f), {1, 0, 0}) * view;
    view = glm::translate(identity, {0, 0, -10}) * view;
    return view;
  })};

  // view-to-clip matrix (projection)
  float fov{45.0f};
  const std::vector<float> aspect_ratios{
      1.0f, 4.0f / 3.0f, 8.0f / 5.0f, 16.0f / 9.0f, 2.0f,
  };
  std::size_t i_aspect_ratio{1};
  glm::mat4 view2clip_perspective() {
    return glm::perspective(glm::radians(fov), aspect_ratios[i_aspect_ratio],
                            0.1f, 100.0f);
  }
  const glm::mat4 view2clip_orthogonal{
      glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, 0.1f, 100.0f)};
  bool use_perspective{true};

 public:
  CoordinateSystems() : Application(800, 600, "Coordinate Systems") {
    program.with_uniform("u_world2view", world2view);
    program.with_uniform("u_view2clip", view2clip_perspective());
    enable_depth_test();
  }

 private:
  void handle_inputs() {
    // move character around with W/A/S/D
    {
      int di = 0;
      int dj = 0;
      di += just_released(iphelf::opengl::Key::S);
      di -= just_released(iphelf::opengl::Key::W);
      dj += just_released(iphelf::opengl::Key::D);
      dj -= just_released(iphelf::opengl::Key::A);
      if (di || dj) {
        int ti = player_location.first + di;
        int tj = player_location.second + dj;
        if (0 <= ti && ti < level.size.first && 0 <= tj &&
            tj < level.size.second && !level.is_block[ti][tj]) {
          player_location.first = ti;
          player_location.second = tj;
          player_cube = translate_on_gird(ti, tj);
        }
      }
    }

    // projection related controls
    {
      bool need_to_refresh_view2clip{false};

      // switch orthogonal/perspective projection with O/P
      if ((just_released(iphelf::opengl::Key::O) && use_perspective) ||
          (just_released(iphelf::opengl::Key::P) && !use_perspective)) {
        use_perspective = !use_perspective;
        need_to_refresh_view2clip = true;
      }

      // adjust FoV with -/+
      if (use_perspective && just_released(iphelf::opengl::Key::EqualPlus)) {
        fov += 10;
        need_to_refresh_view2clip = true;
      }
      if (use_perspective &&
          just_released(iphelf::opengl::Key::MinusUnderscore)) {
        fov -= 10;
        need_to_refresh_view2clip = true;
      }

      // iterate through aspect ratios with TAB
      if (use_perspective && just_released(iphelf::opengl::Key::Tab)) {
        i_aspect_ratio = (i_aspect_ratio + 1) % aspect_ratios.size();
        need_to_refresh_view2clip = true;
      }

      if (need_to_refresh_view2clip)
        program.with_uniform("u_view2clip", use_perspective
                                                ? view2clip_perspective()
                                                : view2clip_orthogonal);
    }

    // move bird's eye camera around with Up/Down/Left/Right
    {
      int di = 0;
      int dj = 0;
      di += just_released(iphelf::opengl::Key::Down);
      di -= just_released(iphelf::opengl::Key::Up);
      dj += just_released(iphelf::opengl::Key::Right);
      dj -= just_released(iphelf::opengl::Key::Left);
      if (di || dj) {
        world2view = world2view * glm::translate(identity, {-dj, 0, -di});
        program.with_uniform("u_world2view", world2view);
      }
    }
  }

  void render() override {
    handle_inputs();

    clear(iphelf::opengl::Colors::DarkGreenBluish);

    program.bind_texture(image_wall);
    program.with_uniform("u_model2world", floor_plane);
    program.render(plane);

    program.bind_texture(image_container);
    for (auto &block_cube : block_cubes) {
      program.with_uniform("u_model2world", block_cube);
      program.render(cube);
    }

    program.bind_texture(image_face);
    static const auto player_scaling{glm::scale(identity, glm::vec3{0.5f})};
    float seconds = elapsed_seconds();
    static const auto rotation_axis{glm::normalize(glm::vec3{1, 0.7, 0.5})};
    auto player_rotation{
        glm::rotate(identity, glm::radians(seconds * 60), rotation_axis)};
    program.with_uniform("u_model2world",
                         player_cube * player_rotation * player_scaling);
    program.render(cube);
  }
};

int main() {
  CoordinateSystems application;
  application.run();
}