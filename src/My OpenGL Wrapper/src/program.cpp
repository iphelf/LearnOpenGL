#include <iphelf/opengl/program.h>

#include <fstream>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <regex>

#include "external/gl.h"
#include "shader.h"

namespace iphelf::opengl {

namespace {

std::string read_file(const std::filesystem::path &path) {
  if (!std::filesystem::is_regular_file(path))
    throw std::runtime_error("Shader file \"" + path.string() +
                             "\" does not exist.");
  std::ostringstream oss;
  oss << std::ifstream(path).rdbuf();
  return oss.str();
}

std::map<std::string, std::string> read_includes(
    const std::map<std::string, std::filesystem::path> &includes) {
  std::map<std::string, std::string> read;
  for (auto [v_path, path] : includes) read[v_path] = read_file(path);
  return read;
}

std::string preprocess_directive_include(
    const std::string &source,
    const std::map<std::string, std::string> &includes) {
  std::ostringstream oss;
  std::regex pattern{
      R"raw(^\s*#include\s*"([^"]+)"\s*$)raw",
      std::regex_constants::ECMAScript | std::regex_constants::multiline};
  std::sregex_iterator matches_begin{source.begin(), source.end(), pattern};
  std::sregex_iterator matches_end;
  auto last_match_end{source.begin()};
  for (auto it{matches_begin}; it != matches_end; ++it) {
    auto &match{*it};
    auto v_path{match.str(1)};
    if (auto found{includes.find(v_path)}; found != includes.end()) {
      auto match_begin{source.begin() + match.position()};
      auto match_end{match_begin + match.length()};
      std::copy(last_match_end, match_begin, std::ostreambuf_iterator{oss});
      oss << found->second;
      last_match_end = match_end;
    }
  }
  std::copy(last_match_end, source.end(), std::ostreambuf_iterator{oss});
  return oss.str();
}

}  // namespace

Program::Program(const std::filesystem::path &path_vertex_shader,
                 const std::filesystem::path &path_fragment_shader) {
  VertexShader vertex_shader{read_file(path_vertex_shader)};
  FragmentShader fragment_shader{read_file(path_fragment_shader)};
  shader_program = gl().create_program_object();
  gl().attach_shader(shader_program, vertex_shader.shader);
  gl().attach_shader(shader_program, fragment_shader.shader);
  gl().link_program(shader_program);
}

Program::Program(
    const std::filesystem::path &path_vertex_shader,
    const std::map<std::string, std::filesystem::path> &vertex_shader_includes,
    const std::filesystem::path &path_fragment_shader,
    const std::map<std::string, std::filesystem::path>
        &fragment_shader_includes) {
  VertexShader vertex_shader{preprocess_directive_include(
      read_file(path_vertex_shader), read_includes(vertex_shader_includes))};
  FragmentShader fragment_shader{
      preprocess_directive_include(read_file(path_fragment_shader),
                                   read_includes(fragment_shader_includes))};
  shader_program = gl().create_program_object();
  gl().attach_shader(shader_program, vertex_shader.shader);
  gl().attach_shader(shader_program, fragment_shader.shader);
  gl().link_program(shader_program);
}

void Program::use() const {
  if (!shader_program) throw std::runtime_error("Empty program is unusable");
  gl().use_program(shader_program);
}

int Program::get_uniform_location(const std::string &name) const {
  return gl().get_uniform_location(shader_program, name);
}

void Program::with_uniform(const std::string &name, const Color &color) const {
  use();
  auto location = get_uniform_location(name);
  gl().set_uniform(location, color);
}

void Program::with_uniform(const std::string &name, int number) const {
  use();
  auto location = get_uniform_location(name);
  gl().set_uniform(location, number);
}

void Program::with_uniform(const std::string &name, float number) const {
  use();
  auto location = get_uniform_location(name);
  gl().set_uniform(location, number);
}

void Program::with_uniform(const std::string &name,
                           const glm::vec3 &vector) const {
  use();
  auto location = get_uniform_location(name);
  gl().set_uniform(location, vector);
}

void Program::with_uniform(const std::string &name,
                           const glm::mat3 &matrix) const {
  use();
  auto location = get_uniform_location(name);
  gl().set_uniform(location, matrix);
}

void Program::with_uniform(const std::string &name,
                           const glm::mat4 &matrix) const {
  use();
  auto location = get_uniform_location(name);
  gl().set_uniform(location, matrix);
}

void Program::bind_texture(const Texture &texture) const {
  use();
  gl().active_texture();
  gl().bind_texture_object(texture.texture_object);
}

void Program::bind_texture(const std::string &name, int slot,
                           const Texture &texture) const {
  use();
  if (slot < 0) throw std::runtime_error("Texture slot must be non-negative");
  gl().active_texture(slot);
  gl().bind_texture_object(texture.texture_object);
  with_uniform(name, slot);
}

void Program::render(const TriangleArray &texture_array) const {
  if (!texture_array.vertex_array_object) return;
  use();
  gl().bind_vertex_array_object(texture_array.vertex_array_object);
  gl().draw_indexed_triangles(texture_array.size);
}

void Program::render_wireframe(const TriangleArray &triangle_array) const {
  gl().set_polygon_mode(GL::PolygonModeTarget::FrontAndBack,
                        GL::PolygonModeType::Line);
  render(triangle_array);
  gl().set_polygon_mode(GL::PolygonModeTarget::FrontAndBack,
                        GL::PolygonModeType::Fill);
}

Program::Program(Program &&other) noexcept {
  std::swap(shader_program, other.shader_program);
}

Program &Program::operator=(Program &&other) noexcept {
  std::swap(shader_program, other.shader_program);
  return *this;
}

Program::~Program() { gl().delete_program_object(shader_program); }

}  // namespace iphelf::opengl
