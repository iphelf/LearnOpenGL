#include "shader.h"

#include "gl.h"

namespace iphelf::opengl {

VertexShader::VertexShader(const std::string &source) {
  shader = gl().create_vertex_shader_object();
  gl().send_shader_source(shader, source);
  gl().compile_shader(shader);
}

VertexShader::~VertexShader() { gl().delete_shader_object(shader); }

FragmentShader::FragmentShader(const std::string &source) {
  shader = gl().create_fragment_shader_object();
  gl().send_shader_source(shader, source);
  gl().compile_shader(shader);
}

FragmentShader::~FragmentShader() { gl().delete_shader_object(shader); }

}  // namespace iphelf::opengl