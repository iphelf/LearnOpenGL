#include "shader.h"

#include <glad/gl.h>

#include <iostream>

namespace iphelf::opengl {

VertexShader::VertexShader(const std::string &source) {
  shader = glCreateShader(GL_VERTEX_SHADER);
  const char *source_data = source.data();
  glShaderSource(shader, 1, &source_data, nullptr);
  glCompileShader(shader);
  GLint success;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    const int info_size = 512;
    char info_log[info_size];
    glGetShaderInfoLog(shader, info_size, nullptr, info_log);
    std::cerr << info_log << std::endl << source << std::endl;
    throw std::runtime_error("Failed to compile vertex shader source");
  }
}

VertexShader::~VertexShader() { glDeleteShader(shader); }

FragmentShader::FragmentShader(const std::string &source) {
  shader = glCreateShader(GL_FRAGMENT_SHADER);
  const char *source_data = source.data();
  glShaderSource(shader, 1, &source_data, nullptr);
  glCompileShader(shader);
  GLint success;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    const int info_size = 512;
    char info_log[info_size];
    glGetShaderInfoLog(shader, info_size, nullptr, info_log);
    std::cerr << info_log << std::endl << source << std::endl;
    throw std::runtime_error("Failed to compile fragment shader source");
  }
}

FragmentShader::~FragmentShader() { glDeleteShader(shader); }

}  // namespace iphelf::opengl