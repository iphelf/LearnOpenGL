#include <iphelf/opengl/application.h>
#include <glad/gl.h>
#include <GLFW/glfw3.h>

namespace iphelf::opengl {

struct Application::Impl {
  static const int padding_left = 0;
  static const int padding_top = 0;
  static const int padding_right = 0;
  static const int padding_bottom = 0;
  GLFWwindow *window{nullptr};
};

Application::Application(int width, int height, const std::string &title) : self(std::make_unique<Impl>()) {
  if (!glfwInit())
    throw std::runtime_error("Failed to initialize GLFW");

  // Create a GLFW window as current context
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#if __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
  self->window = glfwCreateWindow(width, height, title.data(), nullptr, nullptr);
  if (!self->window)
    throw std::runtime_error("Failed to create GLFW window");
  glfwMakeContextCurrent(self->window);

  // Initialize OpenGL with GLAD
  if (!gladLoadGL(glfwGetProcAddress))
    throw std::runtime_error("Failed to initialize GLAD");

  // Setup OpenGL viewport inside the GLFW window
  auto framebuffer_size_callback = [](GLFWwindow *, int width, int height) {
    glViewport(
        Impl::padding_left, Impl::padding_bottom,
        width - Impl::padding_left - Impl::padding_right,
        height - Impl::padding_top - Impl::padding_bottom
    );
  };
  framebuffer_size_callback(nullptr, width, height);
  glfwSetFramebufferSizeCallback(self->window, framebuffer_size_callback);
}

Application::~Application() {
  glfwTerminate();
}

void Application::run() {
  while (!glfwWindowShouldClose(self->window)) {
    render();

    glfwSwapBuffers(self->window);
    glfwPollEvents();
  }
}

void Application::render() {}

VertexArrayBuilder Application::build_vertex_array() {
  return {};
}

ProgramBuilder Application::build_program() {
  return {};
}

void Application::clear(Color color) {
  glClearColor(color.r, color.g, color.b, color.a);
  glClear(GL_COLOR_BUFFER_BIT);
}

} // opengl