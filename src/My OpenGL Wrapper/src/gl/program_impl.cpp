#include "program_impl.h"
#include "../shader.h"
#include "vertices_impl.h"
#include <glad/gl.h>
#include <iostream>

namespace iphelf::opengl {

Program::Impl::Impl(
    std::unique_ptr<VertexShader> vertex_shader,
    std::unique_ptr<FragmentShader> fragment_shader
) {
  shader_program = glCreateProgram();
  if (vertex_shader)
    glAttachShader(shader_program, vertex_shader->shader);
  if (fragment_shader)
    glAttachShader(shader_program, fragment_shader->shader);
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

Program::Impl::~Impl() {
  glDeleteProgram(shader_program);
}

void Program::render(const VertexArray &va) {
  if (!self || !va.self) return;
  glUseProgram(self->shader_program);
  glBindVertexArray(va.self->vertex_array_object);
  glDrawArrays(GL_TRIANGLES, 0, 3);
//  glBindVertexArray(0);
//  glUseProgram(0);
}

}