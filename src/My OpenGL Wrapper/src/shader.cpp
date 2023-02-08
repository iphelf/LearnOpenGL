#include "shader.h"

#include "external/gl.h"

namespace iphelf::opengl {

VertexShader::VertexShader(const std::string &source) {
  shader = gl().create_vertex_shader_object();
  gl().send_shader_source(shader, source);
  gl().compile_shader(shader);
}

VertexShader::VertexShader(
    const std::string &source,
    const std::map<std::string, std::string> &included_sources) {
  shader = gl().create_vertex_shader_object();
  gl().send_shader_source(shader, source);
  if (!included_sources.empty())
    gl().compile_shader_with_include(shader, included_sources);
  else
    gl().compile_shader(shader);
}

VertexShader::~VertexShader() { gl().delete_shader_object(shader); }

FragmentShader::FragmentShader(const std::string &source) {
  shader = gl().create_fragment_shader_object();
  gl().send_shader_source(shader, source);
  gl().compile_shader(shader);
}

FragmentShader::FragmentShader(
    const std::string &source,
    const std::map<std::string, std::string> &included_sources) {
  shader = gl().create_fragment_shader_object();
  gl().send_shader_source(shader, source);
  if (!included_sources.empty())
    gl().compile_shader_with_include(shader, included_sources);
  else
    gl().compile_shader(shader);
}

FragmentShader::~FragmentShader() { gl().delete_shader_object(shader); }

}  // namespace iphelf::opengl