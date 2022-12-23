#pragma once

#include <functional>

namespace iphelf::opengl {

template <typename T>
concept VertexIndexSet =
    requires(T t, std::function<void(unsigned)> f) { t.for_each(f); };

namespace Shapes {

struct Triangle {
  Triangle(unsigned a, unsigned b, unsigned c) : a(a), b(b), c(c) {
    static_assert(VertexIndexSet<Triangle>);
  }
  inline void for_each(const std::function<void(unsigned)>& f) const {
    f(a);
    f(b);
    f(c);
  }
  unsigned a;
  unsigned b;
  unsigned c;
};

}  // namespace Shapes

}  // namespace iphelf::opengl
