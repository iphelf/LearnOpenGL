#pragma once

#include <chrono>
#include <filesystem>
#include <memory>

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
      Texture::WrappingMode wrapping_mode = Texture::WrappingMode::Repeat,
      Texture::FilterType filter_type = Texture::FilterType::Nearest) {
    return {path_texture, wrapping_mode, filter_type};
  }

  static std::chrono::duration<float> get_time();

  // render stuff
  static void clear(const Color &color);
};

}  // namespace iphelf::opengl
