#include <iphelf/opengl/shapearray.h>

#include <list>

#include "gl/vertices_impl.h"

namespace iphelf::opengl {

ShapeArray::ShapeArray() : self(nullptr) {}

ShapeArray::~ShapeArray() = default;

ShapeArray::ShapeArray(std::unique_ptr<Impl> &&impl) : self(std::move(impl)) {}

ShapeArray &ShapeArray::operator=(ShapeArray &&other) noexcept {
  self = std::move(other.self);
  other.self = nullptr;
  return *this;
}

template <VertexIndexSet T>
struct ShapeArrayBuilder<T>::Impl {
  std::vector<float> vertices;
  std::vector<unsigned> elements;
  Impl(const std::initializer_list<Vec3> &vertices)
      : vertices(vertices.size() * 3) {
    size_t i = 0;
    for (const auto &vertex : vertices) {
      this->vertices[i++] = vertex.x;
      this->vertices[i++] = vertex.y;
      this->vertices[i++] = vertex.z;
    }
  }
};

template <VertexIndexSet T>
ShapeArrayBuilder<T>::ShapeArrayBuilder(
    const std::initializer_list<Vec3> &vertices)
    : self(std::make_unique<Impl>(vertices)) {}

template <VertexIndexSet T>
ShapeArrayBuilder<T>::~ShapeArrayBuilder() = default;

template <VertexIndexSet T>
ShapeArrayBuilder<T> &ShapeArrayBuilder<T>::add(const T &shape) {
  shape.for_each([this](unsigned index) {
    if (index >= self->vertices.size())
      throw std::domain_error("Index out of range of available vertices");
    self->elements.push_back(index);
  });
  return *this;
}

template <VertexIndexSet T>
ShapeArrayBuilder<T> &ShapeArrayBuilder<T>::add(
    const std::initializer_list<T> &shapes) {
  for (const auto &shape : shapes) add(shape);
  return *this;
}

template <VertexIndexSet T>
ShapeArray ShapeArrayBuilder<T>::build() {
  return ShapeArray{
      std::make_unique<ShapeArray::Impl>(self->vertices, self->elements)};
}

template class ShapeArrayBuilder<Shapes::Triangle>;

}  // namespace iphelf::opengl
