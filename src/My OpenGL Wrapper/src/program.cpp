#include <iphelf/opengl/program.h>

#include <iostream>

#include "gl/program_impl.h"
#include "gl/vertices_impl.h"
#include "shader.h"

namespace iphelf::opengl {

Program::Program() : self(nullptr) {}

Program &Program::operator=(Program &&other) noexcept {
  self = std::move(other.self);
  other.self = nullptr;
  return *this;
}

Program::Program(std::unique_ptr<Impl> &&impl) : self(std::move(impl)) {}

Program::~Program() = default;

struct ProgramBuilder::Impl {
  std::unique_ptr<VertexShader> vertex_shader{nullptr};
  std::unique_ptr<FragmentShader> fragment_shader{nullptr};
};

ProgramBuilder::ProgramBuilder() : self(std::make_unique<Impl>()) {}

ProgramBuilder::~ProgramBuilder() = default;

ProgramBuilder &ProgramBuilder::set_vertex_shader() {
  self->vertex_shader = std::make_unique<VertexShader>();
  return *this;
}

ProgramBuilder &ProgramBuilder::set_fragment_shader(Color color) {
  self->fragment_shader = std::make_unique<FragmentShader>(color);
  return *this;
}

Program ProgramBuilder::build() {
  return Program(std::make_unique<Program::Impl>(
      std::move(self->vertex_shader), std::move(self->fragment_shader)));
}

}  // namespace iphelf::opengl