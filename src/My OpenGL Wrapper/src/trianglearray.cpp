#include <iphelf/opengl/trianglearray.h>

#include <algorithm>
#include <map>
#include <numeric>

#include "gl.h"

namespace iphelf::opengl {

namespace {

using Element = IndexedTriangle::Vertex;
std::pair<std::vector<float>, std::vector<Element>> reduce(
    std::size_t n_triangles, std::size_t dimension,
    const std::span<const float> &fields) {
  std::map<std::vector<float>, int> elementOfVertex;
  for (std::size_t i{0}; i < n_triangles; ++i) {
    auto triangle = std::span(fields.data() + i * dimension * 3, dimension * 3);
    for (std::size_t j{0}; j < 3; ++j)
      elementOfVertex.emplace(
          std::vector<float>{triangle.data() + dimension * j,
                             triangle.data() + dimension * (j + 1)},
          1);
  }
  auto n_reduced_fields = elementOfVertex.size() * dimension;
  std::vector<float> reduced_fields(n_reduced_fields);
  auto n_elements = n_triangles * 3;
  std::vector<Element> elements(n_elements);
  int cnt = 0;
  for (auto &[vertex, element] : elementOfVertex) {
    element = cnt;
    std::copy_n(vertex.begin(), dimension,
                reduced_fields.data() + dimension * cnt);
    ++cnt;
  }
  for (std::size_t i{0}; i < n_triangles; ++i) {
    auto triangle = std::span(fields.data() + i * dimension * 3, dimension * 3);
    //    auto triangle = fields + i * dimension * 3;
    for (std::size_t j{0}; j < 3; ++j)
      elements[i * 3 + j] = elementOfVertex[std::vector<float>{
          triangle.data() + dimension * j,
          triangle.data() + dimension * (j + 1)}];
  }
  return {std::move(reduced_fields), std::move(elements)};
}

}  // namespace

TriangleArray::TriangleArray(std::size_t n_triangles,
                             const std::vector<int> &attribute_sizes,
                             const std::span<const float> &fields) {
  auto dimension = static_cast<std::size_t>(
      std::accumulate(attribute_sizes.begin(), attribute_sizes.end(), 0));
  auto [reduced_fields, elements] = reduce(n_triangles, dimension, fields);
  init(attribute_sizes, reduced_fields, elements);
}

TriangleArray::TriangleArray(const std::vector<int> &attribute_sizes,
                             const std::span<const float> &fields,
                             const std::span<const Index> &elements) {
  init(attribute_sizes, fields, elements);
}

void TriangleArray::init(const std::vector<int> &attribute_sizes,
                         const std::span<const float> &fields,
                         const std::span<const Index> &elements) {
  vertex_array_object = gl().gen_vertex_array_object();
  vertex_buffer_object = gl().gen_buffer_object();
  element_buffer_object = gl().gen_buffer_object();

  gl().bind_vertex_array_object(vertex_array_object);

  gl().bind_vertex_array_buffer_object(vertex_buffer_object);
  gl().bind_vertex_array_buffer_object(vertex_buffer_object);
  gl().send_vertex_array_buffer_data(fields);

  gl().bind_element_array_buffer_object(element_buffer_object);
  gl().send_element_array_buffer_data(elements);

  size = static_cast<int>(elements.size());

  gl().setup_vertex_attributes(attribute_sizes);

  gl().unbind_vertex_array_buffer_object();
  gl().unbind_vertex_array_object();
}

TriangleArray::~TriangleArray() {
  gl().delete_vertex_array_object(vertex_array_object);
  gl().delete_buffer_object(vertex_buffer_object);
  gl().delete_buffer_object(element_buffer_object);
}

TriangleArray::TriangleArray(TriangleArray &&other) noexcept {
  std::swap(vertex_array_object, other.vertex_array_object);
  std::swap(vertex_buffer_object, other.vertex_buffer_object);
  std::swap(element_buffer_object, other.element_buffer_object);
  std::swap(size, other.size);
}

TriangleArray &TriangleArray::operator=(TriangleArray &&other) noexcept {
  std::swap(vertex_array_object, other.vertex_array_object);
  std::swap(vertex_buffer_object, other.vertex_buffer_object);
  std::swap(element_buffer_object, other.element_buffer_object);
  std::swap(size, other.size);
  return *this;
}

// Template instantiations for compile time check purpose only
template struct GenericTriangle<Vertex<2>>;
template struct GenericTriangle<Element>;
template TriangleArray::TriangleArray(const std::vector<Triangle<2>> &triangles,
                                      const std::vector<int> &attribute_sizes);
template TriangleArray::TriangleArray(
    const std::vector<Vertex<2>> &vertices,
    const std::vector<IndexedTriangle> &triangles,
    const std::vector<int> &attribute_sizes);

}  // namespace iphelf::opengl
