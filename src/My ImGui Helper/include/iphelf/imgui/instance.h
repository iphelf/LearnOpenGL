#pragma once

#include <imgui.h>

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

  // Calls all necessary *NewFrame()
  void new_frame();

  // Calls Render() and *RenderDrawData()
  void render();
};

}  // namespace iphelf::imgui
