#include <glad/gl.h>
#include <iphelf/opengl/texture.h>
#include <stb/stb_image.h>

#include <iostream>
#include <map>
#include <unordered_map>

namespace iphelf::opengl {

namespace {

const std::unordered_map<Texture::WrappingMode, int> gl_wrapping_modes{
    {Texture::WrappingMode::Repeat, GL_REPEAT},
    {Texture::WrappingMode::MirroredRepeat, GL_MIRRORED_REPEAT},
    {Texture::WrappingMode::ClampToEdge, GL_CLAMP_TO_EDGE},
    {Texture::WrappingMode::ClampToBorder, GL_CLAMP_TO_BORDER},
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
                 Texture::FilterType filter_type,
                 Texture::WrappingMode wrapping_mode, Color border_color) {
  int width;
  int height;
  int n_channels;
  stbi_set_flip_vertically_on_load(true);
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
  if (wrapping_mode == WrappingMode::ClampToBorder)
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, &border_color.r);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  gl_mipmap_filter_types.at({filter_type, filter_type}));
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                  gl_filter_types.at(filter_type));
  switch (n_channels) {
    case 3:
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                   GL_UNSIGNED_BYTE, image_data.get());
      break;
    case 4:
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
                   GL_UNSIGNED_BYTE, image_data.get());
      break;
    default:
      throw std::runtime_error("Unusual n_channels: " +
                               std::to_string(n_channels));
  }
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
