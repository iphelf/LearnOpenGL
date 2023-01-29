#include "iphelf/imgui/instance.h"

#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

namespace iphelf::imgui {
Instance::Instance(GLFWwindow *w) : w{w} {
  IMGUI_CHECKVERSION();
  ctx = ImGui::CreateContext();
  ImGui::StyleColorsDark();
  ImGui_ImplGlfw_InitForOpenGL(w, false);
  ImGui_ImplOpenGL3_Init("#version 460");
}

Instance::~Instance() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext(ctx);
}

void Instance::install_callbacks() { ImGui_ImplGlfw_InstallCallbacks(w); }

void Instance::render(std::function<void(void)> &&routine) {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
  routine();
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

}  // namespace iphelf::imgui