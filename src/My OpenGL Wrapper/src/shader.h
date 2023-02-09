#pragma once

#include <filesystem>
#include <map>
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
  //  [[deprecated]] VertexShader(
  //      const std::string &source,
  //      const std::map<std::string, std::string> &included_sources);
  ~VertexShader();
};

class FragmentShader : public Shader {
 public:
  explicit FragmentShader(const std::string &source);
  //  [[deprecated]] FragmentShader(
  //      const std::string &source,
  //      const std::map<std::string, std::string> &included_sources);
  ~FragmentShader();
};

}  // namespace iphelf::opengl
