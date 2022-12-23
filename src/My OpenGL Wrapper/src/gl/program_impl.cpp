#include "program_impl.h"

#include <glad/gl.h>

#include <iostream>

#include "iphelf/opengl/program.h"
#include "vertices_impl.h"

namespace iphelf::opengl {

Program::Impl::Impl(std::unique_ptr<VertexShader> vertex_shader,
                    std::unique_ptr<FragmentShader> fragment_shader) {
  shader_program = glCreateProgram();
  if (vertex_shader) glAttachShader(shader_program, vertex_shader->shader);
  if (fragment_shader) glAttachShader(shader_program, fragment_shader->shader);
  glLinkProgram(shader_program);
  GLint success;
  glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
  if (!success) {
    const int info_size = 512;
    char info_log[info_size];
    glGetProgramInfoLog(shader_program, info_size, nullptr, info_log);
    std::cerr << info_log << std::endl;
    throw std::runtime_error("Failed to link shader program_orange");
  }
}

Program::Impl::~Impl() { glDeleteProgram(shader_program); }

void Program::render(const ShapeArray &sa) {
  if (!self || !sa.self) return;
  glUseProgram(self->shader_program);
  glBindVertexArray(sa.self->vertex_array_object);
  glDrawElements(GL_TRIANGLES, sa.self->ssize(), GL_UNSIGNED_INT, nullptr);
  //  glDrawArrays(GL_TRIANGLES, 0, sa.self->ssize());
  glBindVertexArray(0);
  glUseProgram(0);
}

void Program::render_wireframe(const ShapeArray &sa) {
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  render(sa);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

}  // namespace iphelf::opengl