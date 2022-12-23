#pragma once

#include <memory>

#include "shapes.h"
#include "vector.h"

namespace iphelf::opengl {

// Wrapper for an indexed vertex array together with related buffers
class ShapeArray {
  struct Impl;
  std::unique_ptr<Impl> self;

  template <VertexIndexSet T>
  friend class ShapeArrayBuilder;

  friend class Program;

 public:
  ShapeArray();
  ~ShapeArray();
  ShapeArray &operator=(ShapeArray &&other) noexcept;
  explicit ShapeArray(std::unique_ptr<Impl> &&impl);
};

template <VertexIndexSet T>
class ShapeArrayBuilder {
  struct Impl;
  std::unique_ptr<Impl> self;
  ShapeArrayBuilder(const std::initializer_list<Vec3> &vertices);
  friend class Application;

 public:
  ~ShapeArrayBuilder();

  ShapeArrayBuilder &add(const T &shape);
  ShapeArrayBuilder &add(const std::initializer_list<T> &shape);

  template <typename... Args>
  inline ShapeArrayBuilder &emplace(Args &&...args) {
    return add(T(std::forward<Args>(args)...));
  }

  ShapeArray build();
};

}  // namespace iphelf::opengl
