#include <glad/gl.h>
#include <iphelf/opengl/program.h>

#include <fstream>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <sstream>

#include "shader.h"

namespace iphelf::opengl {

Program::Program(const std::filesystem::path &path_vertex_shader,
                 const std::filesystem::path &path_fragment_shader) {
  auto read_file = [](const std::filesystem::path &path) {
    if (!std::filesystem::is_regular_file(path))
      throw std::runtime_error("Shader file \"" + path.string() +
                               "\" does not exist.");
    std::ostringstream oss;
    oss << std::ifstream(path).rdbuf();
    return oss.str();
  };
  // load vertex shader
  const std::string source_vertex_shader = read_file(path_vertex_shader);
  auto vertex_shader = VertexShader(source_vertex_shader);
  // load fragment shader
  const std::string source_fragment_shader = read_file(path_fragment_shader);
  auto fragment_shader = FragmentShader(source_fragment_shader);

  shader_program = glCreateProgram();
  glAttachShader(shader_program, vertex_shader.shader);
  glAttachShader(shader_program, fragment_shader.shader);
  glLinkProgram(shader_program);
  GLint success;
  glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
  if (!success) {
    const int info_size = 512;
    char info_log[info_size];
    glGetProgramInfoLog(shader_program, info_size, nullptr, info_log);
    std::cerr << info_log << std::endl;
    throw std::runtime_error("Failed to link shader program");
  }
}

void Program::use() const {
  if (!shader_program) throw std::runtime_error("Empty program is unusable");
  glUseProgram(shader_program);
}

int Program::get_uniform_location(const std::string &name) const {
  auto location{glGetUniformLocation(shader_program, name.data())};
  if (location < 0)
    throw std::runtime_error("Uniform `" + name + "` not found in shaders");
  return location;
}

void Program::with_uniform(const std::string &name, Color color) const {
  use();
  auto location = get_uniform_location(name);
  glUniform4f(location, color.r, color.g, color.b, color.a);
}

void Program::with_uniform(const std::string &name, int number) const {
  use();
  auto location = get_uniform_location(name);
  glUniform1i(location, number);
}

void Program::with_uniform(const std::string &name, float number) const {
  use();
  auto location = get_uniform_location(name);
  glUniform1f(location, number);
}

void Program::with_uniform(const std::string &name,
                           const glm::mat4 &matrix) const {
  use();
  auto location = get_uniform_location(name);
  glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

void Program::bind_texture(const Texture &texture) const {
  use();
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture.texture_object);
}

void Program::bind_texture(const std::string &name, int slot,
                           const Texture &texture) const {
  use();
  if (slot < 0) throw std::runtime_error("Texture slot must be non-negative");
  glActiveTexture(GL_TEXTURE0 + slot);
  glBindTexture(GL_TEXTURE_2D, texture.texture_object);
  with_uniform(name, slot);
}

void Program::render(const TriangleArray &texture_array) const {
  if (!texture_array.vertex_array_object) return;
  use();
  glBindVertexArray(texture_array.vertex_array_object);
  glDrawElements(GL_TRIANGLES, texture_array.size, GL_UNSIGNED_INT, nullptr);
}

void Program::render_wireframe(const TriangleArray &triangle_array) const {
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  render(triangle_array);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

Program::Program(Program &&other) noexcept {
  std::swap(shader_program, other.shader_program);
}

Program &Program::operator=(Program &&other) noexcept {
  std::swap(shader_program, other.shader_program);
  return *this;
}

Program::~Program() {
  if (shader_program) glDeleteProgram(shader_program);
}

}  // namespace iphelf::opengl
