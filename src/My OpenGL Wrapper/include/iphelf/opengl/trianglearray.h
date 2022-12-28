#pragma once

#include <array>
#include <vector>

namespace iphelf::opengl {

template <std::size_t Dimension>
using Vertex = std::array<float, Dimension>;

// TODO: try 2D std::array

template <std::size_t Dimension>
using Triangle =
    std::tuple<Vertex<Dimension>, Vertex<Dimension>, Vertex<Dimension>>;
//    std::array<Vertex<Dimension>, 3>;

using IndexedTriangle = std::tuple<int, int, int>;
//    std::array<int, 3>;

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
  using Element=unsigned;
  void init(const std::vector<int> &attribute_sizes,
            const std::vector<float> &fields, const std::vector<Element> &elements);

 public:
  TriangleArray() = default;
  TriangleArray(const TriangleArray &) = delete;
  TriangleArray &operator=(const TriangleArray &) = delete;
  TriangleArray(TriangleArray &&other) noexcept;
  TriangleArray &operator=(TriangleArray &&other) noexcept;
  ~TriangleArray();
};

}  // namespace iphelf::opengl
