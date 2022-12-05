#include <iphelf/opengl/vertices.h>
#include <list>
#include "gl/vertices_impl.h"

namespace iphelf::opengl {

VertexArray::VertexArray() : self(nullptr) {}

VertexArray::~VertexArray() = default;

VertexArray::VertexArray(std::unique_ptr<Impl> &&impl) : self(std::move(impl)) {}

VertexArray &VertexArray::operator=(VertexArray &&other) noexcept {
  self = std::move(other.self);
  other.self = nullptr;
  return *this;
}

struct VertexArrayBuilder::Impl {
  std::list<float> vertices;
};

VertexArrayBuilder &VertexArrayBuilder::add_vec3(Vec3 v) {
  self->vertices.emplace_back(v.x);
  self->vertices.emplace_back(v.y);
  self->vertices.emplace_back(v.z);
  return *this;
}

VertexArrayBuilder &VertexArrayBuilder::add_triangle(Vec3 a, Vec3 b, Vec3 c) {
  add_vec3(a);
  add_vec3(b);
  add_vec3(c);
  return *this;
}

VertexArrayBuilder::VertexArrayBuilder() : self(std::make_unique<Impl>()) {}

VertexArray VertexArrayBuilder::build() {
  return VertexArray{std::make_unique<VertexArray::Impl>(
      std::vector(self->vertices.begin(), self->vertices.end())
  )};
}

VertexArrayBuilder::~VertexArrayBuilder() = default;

}
