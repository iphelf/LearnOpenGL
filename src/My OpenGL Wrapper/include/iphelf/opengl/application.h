#pragma once

#include <memory>
#include "vertices.h"
#include "program.h"

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
  static VertexArrayBuilder build_vertex_array();
  static ProgramBuilder build_program();

  // render stuff
  static void clear(Color color);
};

} // opengl
