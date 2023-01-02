#pragma once

#include <filesystem>

namespace iphelf::opengl {

class Texture {
  int texture_object{0};

 public:
  enum class WrappingMode {
    Repeat,
    MirroredRepeat,
    ClampToEdge,
    ClampToBorder,
  };
  enum class FilterType {
    Nearest,
    Linear,
  };

 private:
  friend class Program;
  friend class Application;

  Texture(const std::filesystem::path &path_texture, WrappingMode wrapping_mode,
          FilterType filter_type);

 public:
  Texture() = default;
  Texture(const Texture &) = delete;
  Texture &operator=(const Texture &) = delete;
  Texture(Texture &&other) noexcept;
  Texture &operator=(Texture &&other) noexcept;
  ~Texture();
};

}  // namespace iphelf::opengl
