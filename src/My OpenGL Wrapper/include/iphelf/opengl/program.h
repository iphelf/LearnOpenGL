#pragma once

#include <memory>
#include "color.h"
#include "vertices.h"

namespace iphelf::opengl {

class Program {
  struct Impl;
  std::unique_ptr<Impl> self;
  friend class ProgramBuilder;
 public:
  Program();
  ~Program();
  Program &operator=(Program &&other) noexcept;
  explicit Program(std::unique_ptr<Impl> &&impl);
  void render(const VertexArray &va);
};

class ProgramBuilder {
  struct Impl;
  std::unique_ptr<Impl> self;
  ProgramBuilder();
  friend class Application;
 public:
  ~ProgramBuilder();
  ProgramBuilder &set_vertex_shader(int vertex_size);
  ProgramBuilder &set_fragment_shader(Color color);
  Program build();
};

}
