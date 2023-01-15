#pragma once

#include <chrono>
#include <filesystem>
#include <memory>

#include "input.h"
#include "program.h"
#include "texture.h"
#include "trianglearray.h"

namespace iphelf::opengl {

class Application {
  struct Impl;
  std::unique_ptr<Impl> self;

 public:
  Application(int width, int height, const std::string &title);
  virtual ~Application();

  // Start this OpenGL-based application
  void run();

 private:
  // Render a single frame
  virtual void render() {}

 protected:
  // things that clients want to do

  // build stuff

  template <int Dimension>
  inline static TriangleArray create_triangle_array(
      const std::vector<Triangle<Dimension>> &triangles,
      const std::vector<int> &attribute_sizes = {Dimension}) {
    return TriangleArray{triangles, attribute_sizes};
  }

  template <int Dimension>
  inline static TriangleArray create_triangle_array(
      const std::vector<Vertex<Dimension>> &vertices,
      const std::vector<IndexedTriangle> &triangles,
      const std::vector<int> &attribute_sizes = {Dimension}) {
    return TriangleArray{vertices, triangles, attribute_sizes};
  }

  inline static Program create_program(
      const std::filesystem::path &path_vertex_shader,
      const std::filesystem::path &path_fragment_shader) {
    return {path_vertex_shader, path_fragment_shader};
  }

  inline static Texture create_texture(
      const std::filesystem::path &path_texture,
      Texture::WrappingMode wrapping_mode = Texture::WrappingMode::ClampToEdge,
      Color border_color = Color::Black,
      Texture::FilterType filter_type = Texture::FilterType::Linear) {
    return {path_texture, filter_type, wrapping_mode, border_color};
  }

  inline static Texture create_texture(
      const std::filesystem::path &path_texture,
      Texture::WrappingMode wrapping_mode, Texture::FilterType filter_type) {
    return {path_texture, filter_type, wrapping_mode, Color::Black};
  }

  // check keyboard
  bool is_down(Key key);
  bool just_released(Key key);

  // check time (in seconds)
  static std::chrono::duration<float> get_time();

  // render stuff
  static void clear(const Color &color);
};

}  // namespace iphelf::opengl
