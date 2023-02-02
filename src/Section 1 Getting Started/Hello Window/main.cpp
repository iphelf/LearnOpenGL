#include <iphelf/opengl/application.h>

class HelloWindow : public iphelf::opengl::Application {
  void render() override { clear(iphelf::opengl::Colors::DarkGreenBluish); }

 public:
  HelloWindow() : iphelf::opengl::Application(800, 600, "Hello Window") {}
};

int main() {
  HelloWindow application;
  application.run();
}
