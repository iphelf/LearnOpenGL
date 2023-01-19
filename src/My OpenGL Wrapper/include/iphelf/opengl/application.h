#pragma once

#include <chrono>
#include <filesystem>
#include <functional>
#include <memory>

#include "camera.h"
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
  static inline TriangleArray create_triangle_array(
      const std::vector<Triangle<Dimension>> &triangles,
      const std::vector<int> &attribute_sizes = {Dimension}) {
    return TriangleArray{triangles, attribute_sizes};
  }

  template <int Dimension>
  static inline TriangleArray create_triangle_array(
      const std::vector<Vertex<Dimension>> &vertices,
      const std::vector<IndexedTriangle> &triangles,
      const std::vector<int> &attribute_sizes = {Dimension}) {
    return TriangleArray{vertices, triangles, attribute_sizes};
  }

  static inline Program create_program(
      const std::filesystem::path &path_vertex_shader,
      const std::filesystem::path &path_fragment_shader) {
    return {path_vertex_shader, path_fragment_shader};
  }

  static inline Texture create_texture(
      const std::filesystem::path &path_texture,
      Texture::WrappingMode wrapping_mode = Texture::WrappingMode::ClampToEdge,
      Color border_color = Color::Black,
      Texture::FilterType filter_type = Texture::FilterType::Linear) {
    return {path_texture, filter_type, wrapping_mode, border_color};
  }

  static inline Texture create_texture(
      const std::filesystem::path &path_texture,
      Texture::WrappingMode wrapping_mode, Texture::FilterType filter_type) {
    return {path_texture, filter_type, wrapping_mode, Color::Black};
  }

  static inline Camera create_camera(const glm::vec3 &pos, const glm::vec3 &up,
                                     const glm::vec3 &front,
                                     float sensitivity = 0.1f) {
    return {pos, up, front, sensitivity};
  }

  // check keyboard
  bool is_down(Key key);
  bool just_released(Key key);
  void enable_cursor_capture(bool enabled = true);
  using CursorPosCallback = std::function<void(double, double)>;
  void add_cursor_pos_callback(CursorPosCallback &&callback);

  // check time (in seconds)
  static float elapsed_seconds();
  float delta_seconds();

  // render stuff
  static void clear(const Color &color);
  static void enable_depth_test(bool enabled = true);
};

}  // namespace iphelf::opengl
