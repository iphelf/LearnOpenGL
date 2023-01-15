// clang-format off
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <unordered_map>
// clang-format on
#include <iphelf/opengl/application.h>

namespace iphelf::opengl {

struct Application::Impl {
  static const int padding_left = 0;
  static const int padding_top = 0;
  static const int padding_right = 0;
  static const int padding_bottom = 0;
  static const std::unordered_map<Key, int> gl_keys;
  GLFWwindow *window{nullptr};
};

Application::Application(int width, int height, const std::string &title)
    : self(std::make_unique<Impl>()) {
  if (!glfwInit()) throw std::runtime_error("Failed to initialize GLFW");

  // Create a GLFW window as current context
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#if __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
  self->window =
      glfwCreateWindow(width, height, title.data(), nullptr, nullptr);
  if (!self->window) throw std::runtime_error("Failed to create GLFW window");
  glfwMakeContextCurrent(self->window);

  // Initialize OpenGL with GLAD
  if (!gladLoadGL(glfwGetProcAddress))
    throw std::runtime_error("Failed to initialize GLAD");

  // Setup OpenGL viewport inside the GLFW window
  auto framebuffer_size_callback = [](GLFWwindow *, int width, int height) {
    glViewport(Impl::padding_left, Impl::padding_bottom,
               width - Impl::padding_left - Impl::padding_right,
               height - Impl::padding_top - Impl::padding_bottom);
  };
  framebuffer_size_callback(nullptr, width, height);
  glfwSetFramebufferSizeCallback(self->window, framebuffer_size_callback);
}

Application::~Application() { glfwTerminate(); }

void Application::run() {
  glEnable(GL_DEPTH_TEST);
  while (!glfwWindowShouldClose(self->window)) {
    if (glfwGetKey(self->window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
      glfwSetWindowShouldClose(self->window, true);
    render();

    glfwSwapBuffers(self->window);
    glfwPollEvents();
  }
}

const std::unordered_map<Key, int> Application::Impl::gl_keys{
    {Key::MinusUnderscore, GLFW_KEY_MINUS},
    {Key::EqualPlus, GLFW_KEY_EQUAL},
    {Key::Up, GLFW_KEY_UP},
    {Key::Down, GLFW_KEY_DOWN},
    {Key::Left, GLFW_KEY_LEFT},
    {Key::Right, GLFW_KEY_RIGHT},
    {Key::Tab, GLFW_KEY_TAB},
    {Key::_0, GLFW_KEY_0},
    {Key::_1, GLFW_KEY_1},
    {Key::_2, GLFW_KEY_2},
    {Key::_3, GLFW_KEY_3},
    {Key::_4, GLFW_KEY_4},
    {Key::_5, GLFW_KEY_5},
    {Key::_6, GLFW_KEY_6},
    {Key::_7, GLFW_KEY_7},
    {Key::_8, GLFW_KEY_8},
    {Key::_9, GLFW_KEY_9},
    {Key::O, GLFW_KEY_O},
    {Key::P, GLFW_KEY_P},
    {Key::W, GLFW_KEY_W},
    {Key::A, GLFW_KEY_A},
    {Key::S, GLFW_KEY_S},
    {Key::D, GLFW_KEY_D},
};

bool Application::is_down(Key key) {
  auto glfw_key = Impl::gl_keys.at(key);
  bool pressed = glfwGetKey(self->window, glfw_key) == GLFW_PRESS;
  return pressed;
}

bool Application::just_released(Key key) {
  static std::unordered_map<Key, bool> key_pressed;
  bool pressed_before = key_pressed[key];
  bool pressed_now = is_down(key);
  key_pressed[key] = pressed_now;
  return pressed_before && !pressed_now;
}

std::chrono::duration<float> Application::get_time() {
  std::chrono::duration<float> seconds{static_cast<float>(glfwGetTime())};
  return seconds;
}

void Application::clear(const Color &color) {
  glClearColor(color.r, color.g, color.b, color.a);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

}  // namespace iphelf::opengl
