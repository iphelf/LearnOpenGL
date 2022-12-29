#pragma once

#include <array>
#include <vector>

namespace iphelf::opengl {

template <std::size_t Dimension>
using Vertex = std::array<float, Dimension>;

template <typename V>
struct GenericTriangle {
  std::array<V, 3> vertices;
  GenericTriangle(const V &a, const V &b, const V &c) : vertices{a, b, c} {}
  GenericTriangle(V &&a, V &&b, V &&c)
      : vertices{std::move(a), std::move(b), std::move(c)} {}
  V &operator[](int i) { return vertices[i]; }
  const V &operator[](int i) const { return vertices[i]; }
  auto begin() { return std::begin(vertices); }
  auto end() { return std::end(vertices); }
  auto begin() const { return std::cbegin(vertices); }
  auto end() const { return std::cend(vertices); }
};

template <std::size_t Dimension>
using Triangle = GenericTriangle<Vertex<Dimension>>;

using IndexedTriangle = GenericTriangle<int>;

class TriangleArray {
  int vertex_array_object = 0;
  int vertex_buffer_object = 0;
  int element_buffer_object = 0;
  int size = 0;

  friend class Application;
  friend class Program;

  template <std::size_t Dimension>
  explicit TriangleArray(const std::vector<Triangle<Dimension>> &triangles,
                         const std::vector<int> &attribute_sizes = {Dimension});
  template <std::size_t Dimension>
  TriangleArray(const std::vector<Vertex<Dimension>> &vertices,
                const std::vector<IndexedTriangle> &triangles,
                const std::vector<int> &attribute_sizes = {Dimension});

  using Element = unsigned;
  void init(const std::vector<int> &attribute_sizes,
            const std::vector<float> &fields,
            const std::vector<Element> &elements);

 public:
  TriangleArray() = default;
  TriangleArray(const TriangleArray &) = delete;
  TriangleArray &operator=(const TriangleArray &) = delete;
  TriangleArray(TriangleArray &&other) noexcept;
  TriangleArray &operator=(TriangleArray &&other) noexcept;
  ~TriangleArray();
};

}  // namespace iphelf::opengl
