#include "gl.h"

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <map>
#include <numeric>

#include "glfw.h"

namespace iphelf::opengl {

GL &gl() {
  static GL instance;
  return instance;
}

GL::GL() {
  // Initialize OpenGL with GLAD
  if (!gladLoadGLLoader(
          reinterpret_cast<GLADloadproc>(glfw().get_proc_address)))
    throw std::runtime_error("Failed to initialize GLAD");
}

GL::~GL() = default;

void GL::viewport(int left, int bottom, int width, int height) {
  glViewport(left, bottom, width, height);
}

void GL::enable_depth_test(bool enabled) {
  if (enabled)
    glEnable(GL_DEPTH_TEST);
  else
    glDisable(GL_DEPTH_TEST);
}

void GL::set_clear_color(const Color &color) {
  glClearColor(color.r, color.g, color.b, color.a);
}

void GL::clear() { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }

void GL::draw_indexed_triangles(int n_indices) {
  glDrawElements(GL_TRIANGLES, n_indices, GL_UNSIGNED_INT, nullptr);
}

void GL::set_polygon_mode(GL::PolygonModeTarget target,
                          GL::PolygonModeType type) {
  static const std::unordered_map<PolygonModeTarget, int> gl_pm_targets{
      {PolygonModeTarget::FrontAndBack, GL_FRONT_AND_BACK},
  };
  static const std::unordered_map<PolygonModeType, int> gl_pm_types{
      {PolygonModeType::Line, GL_LINE},
      {PolygonModeType::Fill, GL_FILL},
  };
  glPolygonMode(gl_pm_targets.at(target), gl_pm_types.at(type));
}

int GL::gen_vertex_array_object() {
  GLuint vao;
  glGenVertexArrays(1, &vao);
  return static_cast<int>(vao);
}

void GL::delete_vertex_array_object(int vao) {
  if (auto vertex_array_object{static_cast<GLuint>(vao)})
    glDeleteVertexArrays(1, &vertex_array_object);
}

int GL::gen_buffer_object() {
  GLuint bo;
  glGenBuffers(1, &bo);
  return static_cast<int>(bo);
}

void GL::delete_buffer_object(int bo) {
  if (auto buffer_object{static_cast<GLuint>(bo)})
    glDeleteBuffers(1, &buffer_object);
}

void GL::bind_vertex_array_object(int vao) { glBindVertexArray(vao); }

void GL::unbind_vertex_array_object() { bind_vertex_array_object(0); }

void GL::bind_vertex_array_buffer_object(int vbo) {
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
}

void GL::unbind_vertex_array_buffer_object() {
  bind_vertex_array_buffer_object(0);
}

void GL::send_vertex_array_buffer_data(const std::span<const float> &fields) {
  glBufferData(GL_ARRAY_BUFFER,
               static_cast<GLsizeiptr>(fields.size() * sizeof(float)),
               fields.data(), GL_STATIC_DRAW);
}

void GL::bind_element_array_buffer_object(int ebo) {
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
}

void GL::unbind_element_array_buffer_object() {
  bind_element_array_buffer_object(0);
}

void GL::send_element_array_buffer_data(
    const std::span<const Index> &elements) {
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,
               static_cast<GLsizeiptr>(elements.size() * sizeof(Index)),
               elements.data(), GL_STATIC_DRAW);
}
void GL::setup_vertex_attributes(const std::vector<int> &attribute_sizes) {
  const GLsizei stride =
      std::accumulate(attribute_sizes.begin(), attribute_sizes.end(), 0) *
      static_cast<int>(sizeof(float));
  for (std::size_t index{0}, offset{0}; index < attribute_sizes.size();
       offset += attribute_sizes[index++]) {
    auto ugly_offset{reinterpret_cast<const void *>(offset * sizeof(float))};
    glVertexAttribPointer(index, attribute_sizes[index], GL_FLOAT, GL_FALSE,
                          stride, ugly_offset);
    glEnableVertexAttribArray(index);
  }
}

int GL::gen_texture_object() {
  GLuint to;
  glGenTextures(1, &to);
  return static_cast<int>(to);
}

void GL::delete_texture_object(int to) {
  if (auto texture_object{static_cast<GLuint>(to)})
    glDeleteTextures(1, &texture_object);
}

void GL::bind_texture_object(int to) { glBindTexture(GL_TEXTURE_2D, to); }

void GL::set_texture_wrapping_mode(Texture::WrappingMode wrapping_mode) {
  static const std::unordered_map<Texture::WrappingMode, int> gl_wrapping_modes{
      {Texture::WrappingMode::Repeat, GL_REPEAT},
      {Texture::WrappingMode::MirroredRepeat, GL_MIRRORED_REPEAT},
      {Texture::WrappingMode::ClampToEdge, GL_CLAMP_TO_EDGE},
      {Texture::WrappingMode::ClampToBorder, GL_CLAMP_TO_BORDER},
  };
  auto gl_wrapping_mode = gl_wrapping_modes.at(wrapping_mode);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, gl_wrapping_mode);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, gl_wrapping_mode);
}

void GL::set_texture_border_color(const Color &color) {
  glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, &color.r);
}

void GL::set_texture_filter_type(Texture::FilterType filter_type) {
  static const std::unordered_map<Texture::FilterType, int> gl_filter_types{
      {Texture::FilterType::Nearest, GL_NEAREST},
      {Texture::FilterType::Linear, GL_LINEAR},
  };
  static const std::map<std::pair<Texture::FilterType, Texture::FilterType>,
                        int>
      gl_mipmap_filter_types{
          {{Texture::FilterType::Nearest, Texture::FilterType::Nearest},
           GL_NEAREST_MIPMAP_NEAREST},
          {{Texture::FilterType::Nearest, Texture::FilterType::Linear},
           GL_NEAREST_MIPMAP_LINEAR},
          {{Texture::FilterType::Linear, Texture::FilterType::Nearest},
           GL_LINEAR_MIPMAP_NEAREST},
          {{Texture::FilterType::Linear, Texture::FilterType::Linear},
           GL_LINEAR_MIPMAP_LINEAR},
      };
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  gl_mipmap_filter_types.at({filter_type, filter_type}));
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                  gl_filter_types.at(filter_type));
}

void GL::send_texture_image(int width, int height, int n_channels,
                            const unsigned char *data) {
  switch (n_channels) {
    case 3:
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                   GL_UNSIGNED_BYTE, data);
      break;
    case 4:
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
                   GL_UNSIGNED_BYTE, data);
      break;
    default:
      throw std::runtime_error("Unusual n_channels: " +
                               std::to_string(n_channels));
  }
}

void GL::generate_mipmap() { glGenerateMipmap(GL_TEXTURE_2D); }

void GL::active_texture(int slot) { glActiveTexture(GL_TEXTURE0 + slot); }

int GL::create_vertex_shader_object() {
  return glCreateShader(GL_VERTEX_SHADER);
}

int GL::create_fragment_shader_object() {
  return glCreateShader(GL_FRAGMENT_SHADER);
}

void GL::delete_shader_object(int so) { glDeleteShader(so); }

void GL::send_shader_source(int so, const std::string &source) {
  const char *source_data = source.data();
  glShaderSource(so, 1, &source_data, nullptr);
}

void GL::compile_shader(int so) {
  glCompileShader(so);
  GLint success;
  glGetShaderiv(so, GL_COMPILE_STATUS, &success);
  if (!success) {
    const int info_size = 512;
    char info_log[info_size];
    glGetShaderInfoLog(so, info_size, nullptr, info_log);
    std::cerr << info_log << '\n' << so << '\n';
    throw std::runtime_error("Failed to compile shader source");
  }
}

int GL::create_program_object() { return glCreateProgram(); }

void GL::delete_program_object(int po) {
  if (po) glDeleteProgram(po);
}

void GL::attach_shader(int po, int so) { glAttachShader(po, so); }

void GL::link_program(int po) {
  glLinkProgram(po);
  GLint success;
  glGetProgramiv(po, GL_LINK_STATUS, &success);
  if (!success) {
    const int info_size = 512;
    char info_log[info_size];
    glGetProgramInfoLog(po, info_size, nullptr, info_log);
    std::cerr << info_log << '\n';
    throw std::runtime_error("Failed to link shader program");
  }
}

void GL::use_program(int po) { glUseProgram(po); }

int GL::get_uniform_location(int po, const std::string &name) {
  auto location{glGetUniformLocation(po, name.data())};
  if (location < 0)
    throw std::runtime_error("Uniform `" + name + "` not found in shaders");
  return location;
}

void GL::set_uniform(int location, const Color &color) {
  glUniform4f(location, color.r, color.g, color.b, color.a);
}

void GL::set_uniform(int location, int number) {
  glUniform1i(location, number);
}

void GL::set_uniform(int location, float number) {
  glUniform1f(location, number);
}

void GL::set_uniform(int location, const glm::mat4 &matrix) {
  glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

}  // namespace iphelf::opengl