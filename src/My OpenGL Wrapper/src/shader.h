#pragma once

#include <memory>
#include "iphelf/opengl/color.h"

namespace iphelf::opengl {

class Shader {
  friend class Program;
 protected:
  unsigned shader = 0;
};

class VertexShader : public Shader {
 public:
  explicit VertexShader(int vertex_size);
  ~VertexShader();
};

class FragmentShader : public Shader {
 public:
  explicit FragmentShader(Color color);
  ~FragmentShader();
};

}
