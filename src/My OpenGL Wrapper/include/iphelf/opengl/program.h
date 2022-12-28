#pragma once

#include <filesystem>

#include "color.h"
#include "trianglearray.h"

namespace iphelf::opengl {

class Program {
  int shader_program = 0;

  friend class Application;

  Program(const std::filesystem::path &path_vertex_shader,
          const std::filesystem::path &path_fragment_shader);

 public:
  Program() = default;
  Program(const Program &) = delete;
  Program &operator=(const Program &) = delete;
  Program(Program &&other) noexcept;
  Program &operator=(Program &&other) noexcept;
  ~Program();

  void with_uniform(const std::string &name, Color color) const;
  void render(const TriangleArray &ta) const;
  void render_wireframe(const TriangleArray &ta) const;
};

}  // namespace iphelf::opengl
