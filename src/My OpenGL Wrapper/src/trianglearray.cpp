#include "iphelf/opengl/trianglearray.h"

#include <glad/gl.h>

#include <algorithm>
#include <map>
#include <numeric>
#include <stdexcept>

namespace iphelf::opengl {

template <std::size_t Dimension>
TriangleArray::TriangleArray(const std::vector<Triangle<Dimension>> &triangles,
                             const std::vector<int> &attribute_sizes) {
  if (Dimension !=
      std::accumulate(attribute_sizes.begin(), attribute_sizes.end(), 0))
    throw std::runtime_error(
        "Sum of `attribute_sizes` does not meet `Dimension`");
  std::map<Vertex<Dimension>, int> elementOfVertex;
  for (auto &triangle : triangles) {
    elementOfVertex.emplace(std::get<0>(triangle), 1);
    elementOfVertex.emplace(std::get<1>(triangle), 1);
    elementOfVertex.emplace(std::get<2>(triangle), 1);
  }
  std::vector<float> fields(elementOfVertex.size() * Dimension);
  std::vector<Element> elements(triangles.size() * 3);
  int cnt = 0;
  for (auto &[vertex, element] : elementOfVertex) {
    element = cnt;
    std::copy_n(vertex.begin(), Dimension, fields.begin() + cnt * Dimension);
    ++cnt;
  }
  for (std::size_t i = 0; i < triangles.size(); ++i) {
    elements[i * 3 + 0] = elementOfVertex[std::get<0>(triangles[i])];
    elements[i * 3 + 1] = elementOfVertex[std::get<1>(triangles[i])];
    elements[i * 3 + 2] = elementOfVertex[std::get<2>(triangles[i])];
  }
  init(attribute_sizes, fields, elements);
}

template TriangleArray::TriangleArray(const std::vector<Triangle<2>> &triangles,
                                      const std::vector<int> &attribute_sizes);
template TriangleArray::TriangleArray(const std::vector<Triangle<5>> &triangles,
                                      const std::vector<int> &attribute_sizes);

template <std::size_t Dimension>
TriangleArray::TriangleArray(const std::vector<Vertex<Dimension>> &vertices,
                             const std::vector<IndexedTriangle> &triangles,
                             const std::vector<int> &attribute_sizes) {
  if (Dimension !=
      std::accumulate(attribute_sizes.begin(), attribute_sizes.end(), 0))
    throw std::runtime_error(
        "Sum of `attribute_sizes` does not meet `Dimension`");
  std::vector<float> fields(vertices.size() * Dimension);
  for (std::size_t i{0}; i < vertices.size(); ++i)
    std::copy_n(vertices[i].begin(), Dimension, fields.begin() + i * Dimension);
  std::vector<Element> elements(triangles.size() * 3);
  for (std::size_t i{0}; i < triangles.size(); ++i) {
    elements[i * 3 + 0] = std::get<0>(triangles[i]);
    elements[i * 3 + 1] = std::get<1>(triangles[i]);
    elements[i * 3 + 2] = std::get<2>(triangles[i]);
  }
  init(attribute_sizes, fields, elements);
}

template TriangleArray::TriangleArray(
    const std::vector<Vertex<2>> &vertices,
    const std::vector<IndexedTriangle> &triangles,
    const std::vector<int> &attribute_sizes);
template TriangleArray::TriangleArray(
    const std::vector<Vertex<5>> &vertices,
    const std::vector<IndexedTriangle> &triangles,
    const std::vector<int> &attribute_sizes);

void TriangleArray::init(const std::vector<int> &attribute_sizes,
                         const std::vector<float> &fields,
                         const std::vector<Element> &elements) {
  {
    auto vao{static_cast<GLuint>(vertex_array_object)};
    glGenVertexArrays(1, &vao);
    vertex_array_object = static_cast<int>(vao);
  }
  {
    auto vbo{static_cast<GLuint>(vertex_buffer_object)};
    glGenBuffers(1, &vbo);
    vertex_buffer_object = static_cast<int>(vbo);
  }
  {
    auto ebo{static_cast<GLuint>(element_buffer_object)};
    glGenBuffers(1, &ebo);
    element_buffer_object = static_cast<int>(ebo);
  }

  glBindVertexArray(vertex_array_object);

  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object);
  glBufferData(GL_ARRAY_BUFFER,
               static_cast<GLsizeiptr>(fields.size() * sizeof(float)),
               fields.data(), GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_object);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,
               static_cast<GLsizeiptr>(elements.size() * sizeof(Element)),
               elements.data(), GL_STATIC_DRAW);
  size = static_cast<int>(elements.size());

  const GLsizei stride =
      std::accumulate(attribute_sizes.begin(), attribute_sizes.end(), 0) *
      static_cast<int>(sizeof(float));
  for (std::size_t index{0}, offset{0}; index < attribute_sizes.size();
       offset += attribute_sizes[index++]) {
    auto ugly_offset{reinterpret_cast<const void *>(offset * sizeof(float))};
    glVertexAttribPointer(index, attribute_sizes[index], GL_FLOAT, GL_FALSE,
                          stride, ugly_offset);
    glEnableVertexAttribArray(index);
  }

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

TriangleArray::~TriangleArray() {
  if (auto vao{static_cast<GLuint>(vertex_array_object)})
    glDeleteVertexArrays(1, &vao);
  if (auto vbo{static_cast<GLuint>(vertex_buffer_object)})
    glDeleteBuffers(1, &vbo);
  if (auto ebo{static_cast<GLuint>(element_buffer_object)})
    glDeleteBuffers(1, &ebo);
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

}  // namespace iphelf::opengl
