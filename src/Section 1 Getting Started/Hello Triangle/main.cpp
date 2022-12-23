#include <iphelf/opengl/application.h>

class HelloTriangle : public iphelf::opengl::Application {
 public:
  HelloTriangle() : iphelf::opengl::Application(800, 600, "Hello, Triangle") {
    trianglesA = build_triangle_array({
                                          {0.5f, 0.5f, 0.0f},    // top right
                                          {0.5f, -0.5f, 0.0f},   // bottom right
                                          {-0.5f, -0.5f, 0.0f},  // bottom left
                                          {-0.5f, 0.5f, 0.0f},   // top left
                                      })
                     .emplace(0, 1, 3)
                     .add({1, 2, 3})
                     .build();
    trianglesB = build_triangle_array({
                                          {0.0f, 0.3f, 0.0f},    // top
                                          {-0.3f, -0.2f, 0.0f},  // bottom left
                                          {0.3f, -0.2f, 0.0f},   // bottom right
                                      })
                     .emplace(0, 1, 2)
                     .build();
    program_orange = build_program()
                         .set_vertex_shader()
                         .set_fragment_shader(iphelf::opengl::Color::Orange)
                         .build();
    program_white = build_program()
                        .set_vertex_shader()
                        .set_fragment_shader(iphelf::opengl::Color::White)
                        .build();
  }

 private:
  void render() override {
    clear(iphelf::opengl::Color::DarkGreenBluish);
    program_orange.render_wireframe(trianglesA);
    program_white.render(trianglesB);
  }

  iphelf::opengl::ShapeArray trianglesA;
  iphelf::opengl::ShapeArray trianglesB;
  iphelf::opengl::Program program_orange;
  iphelf::opengl::Program program_white;
};

int main() {
  HelloTriangle application;
  application.run();
}
