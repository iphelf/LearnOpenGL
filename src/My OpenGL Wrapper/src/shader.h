#pragma once

#include <string>

namespace iphelf::opengl {

class Shader {
  friend class Program;

 protected:
  int shader = 0;
};

class VertexShader : public Shader {
 public:
  explicit VertexShader(const std::string &source);
  ~VertexShader();
};

class FragmentShader : public Shader {
 public:
  explicit FragmentShader(const std::string &source);
  ~FragmentShader();
};

}  // namespace iphelf::opengl
