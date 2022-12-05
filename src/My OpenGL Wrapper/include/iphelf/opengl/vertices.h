#pragma once

#include <memory>
#include "vector.h"

namespace iphelf::opengl {

class VertexArray {
  struct Impl;
  std::unique_ptr<Impl> self;
  friend class VertexArrayBuilder;
  friend class Program;
 public:
  VertexArray();
  ~VertexArray();
  VertexArray &operator=(VertexArray &&other) noexcept;
  explicit VertexArray(std::unique_ptr<Impl> &&impl);
};

class VertexArrayBuilder {
  struct Impl;
  std::unique_ptr<Impl> self;
  VertexArrayBuilder();
  friend class Application;
 public:
  ~VertexArrayBuilder();
  VertexArrayBuilder &add_vec3(Vec3 v);
  VertexArrayBuilder &add_triangle(Vec3 a, Vec3 b, Vec3 c);
  VertexArray build();
};

}