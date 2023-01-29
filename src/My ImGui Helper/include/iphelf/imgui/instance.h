#pragma once

#include <imgui.h>

#include <functional>

struct GLFWwindow;

namespace iphelf::imgui {

class Instance {
  GLFWwindow *w;
  ImGuiContext *ctx;

 public:
  explicit Instance(GLFWwindow *w);
  ~Instance();

  // Note: call this after all other callbacks have been installed
  void install_callbacks();

  // Calls all necessary *NewFrame() before,
  // and Render() and *RenderDrawData() after,
  // calling routine()
  void render(std::function<void(void)> &&routine);
};

}  // namespace iphelf::imgui
