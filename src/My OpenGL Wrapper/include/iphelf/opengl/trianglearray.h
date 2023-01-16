#pragma once

#include <array>
#include <span>
#include <vector>

namespace iphelf::opengl {

template <std::size_t Dimension>
using Vertex = std::array<float, Dimension>;

template <typename V>
struct GenericTriangle {
  using Vertex = V;
  std::array<Vertex, 3> vertices;
  GenericTriangle(const Vertex &a, const Vertex &b, const Vertex &c)
      : vertices{a, b, c} {}
  GenericTriangle(Vertex &&a, Vertex &&b, Vertex &&c)
      : vertices{std::move(a), std::move(b), std::move(c)} {}
  Vertex &operator[](int i) { return vertices[i]; }
  const Vertex &operator[](int i) const { return vertices[i]; }
  auto begin() { return std::begin(vertices); }
  auto end() { return std::end(vertices); }
  [[nodiscard]] auto begin() const { return std::cbegin(vertices); }
  [[nodiscard]] auto end() const { return std::cend(vertices); }
};

template <std::size_t Dimension>
using Triangle = GenericTriangle<Vertex<Dimension>>;

using Index = unsigned;
using IndexedTriangle = GenericTriangle<Index>;

class TriangleArray {
  int vertex_array_object = 0;
  int vertex_buffer_object = 0;
  int element_buffer_object = 0;
  int size = 0;

  friend class Application;
  friend class Program;

  template <std::size_t Dimension>
  TriangleArray(const std::vector<Triangle<Dimension>> &triangles,
                const std::vector<int> &attribute_sizes)
      : TriangleArray{
            triangles.size(), attribute_sizes,
            std::span<const float>(triangles.front().vertices.front().data(),
                                   triangles.size() * 3 * Dimension)} {}

  template <std::size_t Dimension>
  TriangleArray(const std::vector<Vertex<Dimension>> &vertices,
                const std::vector<IndexedTriangle> &triangles,
                const std::vector<int> &attribute_sizes)
      : TriangleArray{attribute_sizes,
                      std::span<const float>(vertices.front().data(),
                                             vertices.size() * Dimension),
                      std::span<const Index>(triangles.front().vertices.data(),
                                             triangles.size() * 3)} {}

  TriangleArray(std::size_t n_triangles,
                const std::vector<int> &attribute_sizes,
                const std::span<const float> &fields);

  TriangleArray(const std::vector<int> &attribute_sizes,
                const std::span<const float> &fields,
                const std::span<const Index> &elements);

  void init(const std::vector<int> &attribute_sizes,
            const std::span<const float> &fields,
            const std::span<const Index> &elements);

 public:
  TriangleArray() = default;
  TriangleArray(const TriangleArray &) = delete;
  TriangleArray &operator=(const TriangleArray &) = delete;
  TriangleArray(TriangleArray &&other) noexcept;
  TriangleArray &operator=(TriangleArray &&other) noexcept;
  ~TriangleArray();
};

}  // namespace iphelf::opengl
