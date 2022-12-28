#include <glad/gl.h>
#include <iphelf/opengl/program.h>

#include <fstream>
#include <iostream>
#include <sstream>

#include "shader.h"

namespace iphelf::opengl {

Program::Program(const std::filesystem::path &path_vertex_shader,
                 const std::filesystem::path &path_fragment_shader) {
  auto read_file = [](const std::filesystem::path &path) {
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

void Program::with_uniform(const std::string &name, Color color) const {
  if (!shader_program) return;
  auto location{glGetUniformLocation(shader_program, name.data())};
  if (location < 0)
    throw std::runtime_error("Uniform `" + name + "` not found in shaders");
  glUseProgram(shader_program);
  glUniform4f(location, color.r, color.g, color.b, color.a);
}

void Program::render(const TriangleArray &ta) const {
  if (!shader_program || !ta.vertex_array_object) return;
  glUseProgram(shader_program);
  glBindVertexArray(ta.vertex_array_object);
  glDrawElements(GL_TRIANGLES, ta.size, GL_UNSIGNED_INT, nullptr);
  glBindVertexArray(0);
  glUseProgram(0);
}

void Program::render_wireframe(const TriangleArray &ta) const {
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  render(ta);
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
