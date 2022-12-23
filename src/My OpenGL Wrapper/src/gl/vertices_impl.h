#pragma once

#include <iphelf/opengl/shapearray.h>

#include <vector>

namespace iphelf::opengl {

struct ShapeArray::Impl {
  size_t size = 0;
  unsigned vertex_array_object = 0;
  explicit Impl(const std::vector<float> &vertices,
                const std::vector<unsigned> &elements);
  ~Impl();
  [[nodiscard]] inline int ssize() const { return static_cast<int>(size); }

 private:
  unsigned vertex_buffer_object = 0;
  unsigned element_buffer_object = 0;
};

}  // namespace iphelf::opengl
