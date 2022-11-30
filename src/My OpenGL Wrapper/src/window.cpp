#include "color.h"
#include <iphelf/opengl/window.h>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <stdexcept>

namespace iphelf::opengl {

class GLFW {
 public:
  GLFW() {
    glfwInit();
  }
  ~GLFW() {
    glfwTerminate();
  }
};

struct Window::Impl {
  static const int padding_left = 20;
  static const int padding_top = 20;
  static const int padding_right = 60;
  static const int padding_bottom = 60;
  static const Color background_color;

  GLFW glfw{};
  GLFWwindow *window{nullptr};
};
const Color Window::Impl::background_color = Color::DarkGreenBluish;

Window::Window(int width, int height, const std::string &title)
    : pImpl(std::make_unique<Impl>()) {
  // Create a GLFW window as current context
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#if __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
  pImpl->window = glfwCreateWindow(width, height, title.data(), nullptr, nullptr);
  if (!pImpl->window)
    throw std::runtime_error("Failed to create GLFW window");
  glfwMakeContextCurrent(pImpl->window);

  // Initialize OpenGL with GLAD
  if (!gladLoadGL(glfwGetProcAddress))
    throw std::runtime_error("Failed to initialize GLAD");

  // Setup OpenGL viewport inside the GLFW window
  auto framebuffer_size_callback = [](GLFWwindow *, int width, int height) {
    glViewport(
        Impl::padding_left, Impl::padding_top,
        width - Impl::padding_left - Impl::padding_right,
        height - Impl::padding_top - Impl::padding_bottom
    );
  };
  framebuffer_size_callback(nullptr, width, height);
  glfwSetFramebufferSizeCallback(pImpl->window, framebuffer_size_callback);
}

void Window::run() {
  while (!glfwWindowShouldClose(pImpl->window)) {
    // Inputs
    if (glfwGetKey(pImpl->window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
      glfwSetWindowShouldClose(pImpl->window, true);

    // Rendering commands
    glClearColor(
        Impl::background_color.r, Impl::background_color.g, Impl::background_color.b,
        Impl::background_color.a
    );
    glClear(GL_COLOR_BUFFER_BIT);

    glfwSwapBuffers(pImpl->window);
    glfwPollEvents();
  }
}

Window::~Window() = default;

}