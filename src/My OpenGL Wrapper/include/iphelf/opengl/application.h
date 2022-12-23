#pragma once

#include <memory>

#include "program.h"
#include "shapearray.h"

namespace iphelf::opengl {

class Application {
  struct Impl;
  std::unique_ptr<Impl> self;

 public:
  Application(int width, int height, const std::string &title);
  virtual ~Application();

  // Start this OpenGL-based application
  void run();

 private:
  // Render a single frame
  virtual void render();

 protected:
  // things that clients want to do

  // build stuff

  template <VertexIndexSet T>
  static ShapeArrayBuilder<T> build_shape_array(
      const std::initializer_list<Vec3> &vertices);

  static ShapeArrayBuilder<Shapes::Triangle> build_triangle_array(
      const std::initializer_list<Vec3> &vertices);

  static ProgramBuilder build_program();

  // render stuff
  static void clear(Color color);
};

}  // namespace iphelf::opengl
