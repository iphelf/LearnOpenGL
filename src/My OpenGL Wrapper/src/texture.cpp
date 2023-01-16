#include <iphelf/opengl/texture.h>
#include <stb/stb_image.h>

#include <iostream>
#include <map>

#include "gl.h"

namespace iphelf::opengl {

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
  texture_object = gl().gen_texture_object();
  gl().bind_texture_object(texture_object);
  gl().set_texture_wrapping_mode(wrapping_mode);
  if (wrapping_mode == WrappingMode::ClampToBorder)
    gl().set_texture_border_color(border_color);
  gl().set_texture_filter_type(filter_type);
  gl().send_texture_image(width, height, n_channels, image_data.get());
  gl().generate_mipmap();
}

Texture::Texture(Texture&& other) noexcept {
  std::swap(texture_object, other.texture_object);
}

Texture& Texture::operator=(Texture&& other) noexcept {
  std::swap(texture_object, other.texture_object);
  return *this;
}

Texture::~Texture() { gl().delete_texture_object(texture_object); }

}  // namespace iphelf::opengl
