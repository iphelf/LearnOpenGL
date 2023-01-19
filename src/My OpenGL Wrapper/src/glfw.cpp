#include "glfw.h"

#include <GLFW/glfw3.h>

#include <stdexcept>
#include <unordered_map>

namespace iphelf::opengl {

GLFW &glfw() {
  static GLFW instance;
  return instance;
}

GLFW::GLFW() : get_proc_address{glfwGetProcAddress} {
  if (!glfwInit()) throw std::runtime_error("Failed to initialize GLFW");
}

GLFW::~GLFW() { glfwTerminate(); }

GLFWwindow *GLFW::create_window(int width, int height,
                                const std::string &title) {
  // Create a GLFW window as current context
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#if __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
  GLFWwindow *w{
      glfwCreateWindow(width, height, title.data(), nullptr, nullptr)};
  if (!w) throw std::runtime_error("Failed to create GLFW window");
  glfwMakeContextCurrent(w);
  return w;
}

void GLFW::set_framebuffer_size_callback(GLFWwindow *w,
                                         framebuffer_size_callback_t callback) {
  glfwSetFramebufferSizeCallback(w, callback);
}

void GLFW::destroy_window(GLFWwindow *w) { glfwDestroyWindow(w); }

bool GLFW::window_should_close(GLFWwindow *w) {
  return glfwWindowShouldClose(w);
}

void GLFW::set_window_should_close(GLFWwindow *w) {
  glfwSetWindowShouldClose(w, true);
}

bool GLFW::is_key_down(GLFWwindow *w, Key k) {
  static const std::unordered_map<Key, int> to_glfw_key{
      {Key::Escape, GLFW_KEY_ESCAPE},
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
      {Key::E, GLFW_KEY_E},
      {Key::Q, GLFW_KEY_Q},
  };
  return glfwGetKey(w, to_glfw_key.at(k)) == GLFW_PRESS;
}

bool GLFW::is_mouse_button_down(GLFWwindow *w, MouseButton mb) {
  static const std::unordered_map<MouseButton, int> to_glfw_mouse_button{
      {MouseButton::L, GLFW_MOUSE_BUTTON_LEFT},
      {MouseButton::M, GLFW_MOUSE_BUTTON_MIDDLE},
      {MouseButton::R, GLFW_MOUSE_BUTTON_RIGHT},
  };
  return glfwGetMouseButton(w, to_glfw_mouse_button.at(mb)) == GLFW_PRESS;
}

void GLFW::enable_cursor_capture(GLFWwindow *w, bool enabled) {
  glfwSetInputMode(w, GLFW_CURSOR,
                   enabled ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
}

void GLFW::set_cursor_pos_callback(GLFWwindow *w,
                                   GLFW::cursor_pos_callback_t callback) {
  glfwSetCursorPosCallback(w, callback);
}

void GLFW::swap_buffers(GLFWwindow *w) { glfwSwapBuffers(w); }

void GLFW::poll_events() { glfwPollEvents(); }

std::chrono::duration<double> GLFW::get_time() {
  return std::chrono::duration<double>{glfwGetTime()};
}

}  // namespace iphelf::opengl