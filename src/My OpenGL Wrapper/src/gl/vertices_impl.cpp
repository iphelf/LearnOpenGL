#include "vertices_impl.h"

#include <glad/gl.h>

namespace iphelf::opengl {

ShapeArray::Impl::Impl(const std::vector<float> &vertices,
                       const std::vector<unsigned> &elements) {
  glGenVertexArrays(1, &vertex_array_object);
  glGenBuffers(1, &vertex_buffer_object);
  glGenBuffers(1, &element_buffer_object);

  glBindVertexArray(vertex_array_object);

  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object);
  glBufferData(GL_ARRAY_BUFFER,
               static_cast<GLsizeiptr>(vertices.size() * sizeof(float)),
               vertices.data(), GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_object);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,
               static_cast<GLsizeiptr>(elements.size() * sizeof(unsigned)),
               elements.data(), GL_STATIC_DRAW);
  size = elements.size();

  {
    const GLuint index = 0;
    const GLint n_components = 3;  // Triangle consists of 3 vertices
    const GLenum type = GL_FLOAT;
    const GLboolean normalize = false;
    const GLsizei stride = n_components * sizeof(float);
    const void *offset = nullptr;
    glVertexAttribPointer(index, n_components, type, normalize, stride, offset);
    glEnableVertexAttribArray(index);
  }

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

ShapeArray::Impl::~Impl() {
  if (vertex_array_object) glDeleteVertexArrays(1, &vertex_array_object);
  if (element_buffer_object) glDeleteBuffers(1, &element_buffer_object);
  if (vertex_buffer_object) glDeleteBuffers(1, &vertex_buffer_object);
}

}  // namespace iphelf::opengl
