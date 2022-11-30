#pragma once
#include <memory>

namespace iphelf::opengl {

class Window {
  struct Impl;
  std::unique_ptr<Impl> pImpl{nullptr};
 public:
  Window(int width, int height, const std::string &title);
  ~Window();
  void run();
};

}