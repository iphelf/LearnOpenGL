#pragma once

#include <iphelf/opengl/program.h>
#include <optional>
#include "../shader.h"

namespace iphelf::opengl {

struct Program::Impl {
  unsigned shader_program = 0;
  explicit Impl(
      std::unique_ptr<VertexShader> vertex_shader,
      std::unique_ptr<FragmentShader> fragment_shader);
  ~Impl();
};

}
