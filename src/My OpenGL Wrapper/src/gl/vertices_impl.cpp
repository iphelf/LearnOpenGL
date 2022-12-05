#include "vertices_impl.h"
#include <glad/gl.h>
#include <iostream>

namespace iphelf::opengl {

VertexArray::Impl::Impl(const std::vector<float> &vertices) {
  glGenBuffers(1, &vertex_buffer_object);
  glGenVertexArrays(1, &vertex_array_object);

  glBindVertexArray(vertex_array_object);
  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object);
  glBufferData(
      GL_ARRAY_BUFFER,
      static_cast<GLsizeiptr>(vertices.size() * sizeof(float)), vertices.data(),
      GL_STATIC_DRAW
  );
  {
    const GLuint index = 0;
    const GLint size = 3;
    const GLenum type = GL_FLOAT;
    const GLboolean normalize = false;
    const GLsizei stride = size * sizeof(float);
    const void *offset = nullptr;
    glVertexAttribPointer(index, size, type, normalize, stride, offset);
    glEnableVertexAttribArray(index);
  }
}

VertexArray::Impl::~Impl() {
  glDeleteVertexArrays(1, &vertex_array_object);
  glDeleteBuffers(1, &vertex_buffer_object);
}
}
