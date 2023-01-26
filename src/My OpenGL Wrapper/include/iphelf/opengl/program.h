#pragma once

#include <filesystem>
#include <glm/fwd.hpp>

#include "color.h"
#include "texture.h"
#include "trianglearray.h"

namespace iphelf::opengl {

class Program {
  int shader_program{0};

  friend class Application;

  Program(const std::filesystem::path &path_vertex_shader,
          const std::filesystem::path &path_fragment_shader);

  void use() const;
  [[nodiscard]] int get_uniform_location(const std::string &name) const;

 public:
  Program() = default;
  Program(const Program &) = delete;
  Program &operator=(const Program &) = delete;
  Program(Program &&other) noexcept;
  Program &operator=(Program &&other) noexcept;
  ~Program();

  void with_uniform(const std::string &name, const Color &color) const;
  void with_uniform(const std::string &name, int number) const;
  void with_uniform(const std::string &name, float number) const;
  void with_uniform(const std::string &name, const glm::vec3 &vector) const;
  void with_uniform(const std::string &name, const glm::mat3 &matrix) const;
  void with_uniform(const std::string &name, const glm::mat4 &matrix) const;
  void bind_texture(const Texture &texture) const;
  void bind_texture(const std::string &name, int slot,
                    const Texture &texture) const;
  void render(const TriangleArray &texture_array) const;
  void render_wireframe(const TriangleArray &triangle_array) const;
};

}  // namespace iphelf::opengl
