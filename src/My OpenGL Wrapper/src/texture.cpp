#include <glad/gl.h>
#include <iphelf/opengl/texture.h>
#include <stb/stb_image.h>

#include <map>
#include <unordered_map>

namespace iphelf::opengl {

namespace {

const std::unordered_map<Texture::WrappingMode, int> gl_wrapping_modes{
    {Texture::WrappingMode::Repeat, GL_REPEAT},
};

const std::unordered_map<Texture::FilterType, int> gl_filter_types{
    {Texture::FilterType::Nearest, GL_NEAREST},
    {Texture::FilterType::Linear, GL_LINEAR},
};

const std::map<std::pair<Texture::FilterType, Texture::FilterType>, int>
    gl_mipmap_filter_types{
        {{Texture::FilterType::Nearest, Texture::FilterType::Nearest},
         GL_NEAREST_MIPMAP_NEAREST},
        {{Texture::FilterType::Nearest, Texture::FilterType::Linear},
         GL_NEAREST_MIPMAP_LINEAR},
        {{Texture::FilterType::Linear, Texture::FilterType::Nearest},
         GL_LINEAR_MIPMAP_NEAREST},
        {{Texture::FilterType::Linear, Texture::FilterType::Linear},
         GL_LINEAR_MIPMAP_LINEAR},
    };

}  // namespace

Texture::Texture(const std::filesystem::path& path_texture,
                 Texture::WrappingMode wrapping_mode,
                 Texture::FilterType filter_type) {
  int width;
  int height;
  int n_channels;
  std::unique_ptr<unsigned char, decltype(&stbi_image_free)> image_data(
      stbi_load(path_texture.c_str(), &width, &height, &n_channels, 0),
      &stbi_image_free);
  if (!image_data)
    throw std::runtime_error("Failed to load image from " +
                             path_texture.string());
  {
    auto to{static_cast<GLuint>(texture_object)};
    glGenTextures(1, &to);
    texture_object = static_cast<int>(to);
  }
  glBindTexture(GL_TEXTURE_2D, texture_object);
  auto gl_wrapping_mode = gl_wrapping_modes.at(wrapping_mode);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, gl_wrapping_mode);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, gl_wrapping_mode);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  gl_mipmap_filter_types.at({filter_type, filter_type}));
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                  gl_filter_types.at(filter_type));
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
               GL_UNSIGNED_BYTE, image_data.get());
  glGenerateMipmap(GL_TEXTURE_2D);
}

Texture::Texture(Texture&& other) noexcept {
  std::swap(texture_object, other.texture_object);
}

Texture& Texture::operator=(Texture&& other) noexcept {
  std::swap(texture_object, other.texture_object);
  return *this;
}

Texture::~Texture() {
  if (auto to{static_cast<GLuint>(texture_object)}) glDeleteTextures(1, &to);
}

}  // namespace iphelf::opengl