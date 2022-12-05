#pragma once

#include <iphelf/opengl/vertices.h>
#include <vector>

namespace iphelf::opengl {

struct VertexArray::Impl {
  unsigned vertex_buffer_object = 0;
  unsigned vertex_array_object = 0;
  explicit Impl(const std::vector<float> &vertices);
  ~Impl();
};

}
