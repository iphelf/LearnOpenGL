#pragma once

namespace iphelf::opengl {

struct Color {
  float r;
  float g;
  float b;
  float a;

  explicit Color(float intensity = 0.0f, float alpha = 1.0f)
      : r{intensity}, g{intensity}, b{intensity}, a{alpha} {}

  Color(float red, float green, float blue, float alpha = 1.0f)
      : r(red), g(green), b(blue), a(alpha) {}

  static const Color Black;
  static const Color White;
  static const Color DarkGreenBluish;
  static const Color Orange;
  static const Color Yellow;
};

}  // namespace iphelf::opengl
