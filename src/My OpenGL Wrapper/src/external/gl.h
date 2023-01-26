#pragma once

#include <glm/fwd.hpp>
#include <span>

#include "iphelf/opengl/color.h"
#include "iphelf/opengl/texture.h"
#include "iphelf/opengl/trianglearray.h"

namespace iphelf::opengl {

// Wrapper for all gl/glad functions
class GL {
  GL();
  ~GL();
  friend GL &gl();

 public:
  // rendering
  void viewport(int left, int bottom, int width, int height);
  void enable_depth_test(bool enabled = true);
  void set_clear_color(const Color &color);
  void clear();
  void draw_indexed_triangles(int n_indices);
  enum class PolygonModeTarget {
    FrontAndBack,
  };
  enum class PolygonModeType {
    Line,
    Fill,
  };
  void set_polygon_mode(PolygonModeTarget target, PolygonModeType type);

  // buffers and objects
  int gen_vertex_array_object();
  void delete_vertex_array_object(int vao);
  int gen_buffer_object();
  void delete_buffer_object(int bo);
  void bind_vertex_array_object(int vao);
  void unbind_vertex_array_object();
  void bind_vertex_array_buffer_object(int vbo);
  void unbind_vertex_array_buffer_object();
  void send_vertex_array_buffer_data(const std::span<const float> &fields);
  void bind_element_array_buffer_object(int ebo);
  void unbind_element_array_buffer_object();
  void send_element_array_buffer_data(const std::span<const Index> &elements);
  void setup_vertex_attributes(const std::vector<int> &attribute_sizes);

  // textures
  int gen_texture_object();
  void delete_texture_object(int to);
  void bind_texture_object(int to);
  void set_texture_wrapping_mode(Texture::WrappingMode wrapping_mode);
  void set_texture_border_color(const Color &color);
  void set_texture_filter_type(Texture::FilterType filter_type);
  void send_texture_image(int width, int height, int n_channels,
                          const unsigned char *data);
  void generate_mipmap();
  void active_texture(int slot = 0);

  // shaders and program
  int create_vertex_shader_object();
  int create_fragment_shader_object();
  void delete_shader_object(int so);
  void send_shader_source(int so, const std::string &source);
  void compile_shader(int so);
  int create_program_object();
  void delete_program_object(int po);
  void attach_shader(int po, int so);
  void link_program(int po);
  void use_program(int po);
  int get_uniform_location(int po, const std::string &name);
  void set_uniform(int location, const Color &color);
  void set_uniform(int location, int number);
  void set_uniform(int location, float number);
  void set_uniform(int location, const glm::vec3 &vector);
  void set_uniform(int location, const glm::mat3 &matrix);
  void set_uniform(int location, const glm::mat4 &matrix);
};

GL &gl();

}  // namespace iphelf::opengl
