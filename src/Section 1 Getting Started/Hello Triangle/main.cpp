#include <iphelf/opengl/application.h>

class HelloTriangle : public iphelf::opengl::Application {
 public:
  HelloTriangle() : iphelf::opengl::Application(800, 600, "Hello, Triangle") {
    va = build_vertex_array()
        .add_triangle(
            {-1.0f, -1.0f, 0.0f},
            {1.0f, -1.0f, 0.0f},
            {0.0f, 1.0f, 0.0f})
        .build();
    program = build_program()
        .set_vertex_shader(3)
        .set_fragment_shader(iphelf::opengl::Color::Orange)
        .build();
  }

 private:
  void render() override {
    clear(iphelf::opengl::Color::DarkGreenBluish);
    program.render(va);
  }

  iphelf::opengl::VertexArray va;
  iphelf::opengl::Program program;
};

int main() {
  HelloTriangle application;
  application.run();
}