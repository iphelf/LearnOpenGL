#include "shader.h"

#include <glad/gl.h>

#include <iostream>

namespace iphelf::opengl {

VertexShader::VertexShader() {
  shader = glCreateShader(GL_VERTEX_SHADER);
  const char *vertex_shader_source =
      "#version 460 core\n"
      "layout (location = 0) in vec3 aPos;\n"
      "void main(){\n"
      "  gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
      "}";
  glShaderSource(shader, 1, &vertex_shader_source, nullptr);
  glCompileShader(shader);
  GLint success;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    const int info_size = 512;
    char info_log[info_size];
    glGetShaderInfoLog(shader, info_size, nullptr, info_log);
    std::cerr << info_log << std::endl;
    throw std::runtime_error("Failed to compile vertex shader source");
  }
}

VertexShader::~VertexShader() { glDeleteShader(shader); }

FragmentShader::FragmentShader(Color color) {
  shader = glCreateShader(GL_FRAGMENT_SHADER);
  std::string fragment_shader_source =
      "#version 460 core\n"
      "out vec4 FragColor;\n"
      "void main(){\n"
      "  FragColor = vec4(" +
      std::to_string(color.r) + ", " + std::to_string(color.g) + ", " +
      std::to_string(color.b) + ", " + std::to_string(color.a) + ");\n}";
  const char *s = fragment_shader_source.data();
  glShaderSource(shader, 1, &s, nullptr);
  glCompileShader(shader);
  GLint success;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    const int info_size = 512;
    char info_log[info_size];
    glGetShaderInfoLog(shader, info_size, nullptr, info_log);
    std::cerr << info_log << std::endl;
    throw std::runtime_error("Failed to compile fragment shader source");
  }
}

FragmentShader::~FragmentShader() { glDeleteShader(shader); }

}  // namespace iphelf::opengl