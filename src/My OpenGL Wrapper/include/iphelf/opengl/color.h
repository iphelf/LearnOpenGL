#pragma once

namespace iphelf::opengl {

struct Color {
  float r;
  float g;
  float b;

  explicit Color(float intensity = 0.0f)
      : r{intensity}, g{intensity}, b{intensity} {}

  Color(float red, float green, float blue) : r(red), g(green), b(blue) {}

  // Note:
  //   hue \in [0.0f, 360.0f]
  //   saturation, value \in [0.0f, 1.0f]
  static Color from_hsv(float hue, float saturation, float value);

  static const Color Black;
  static const Color White;
  static const Color DarkGreenBluish;
  static const Color Orange;
  static const Color Yellow;
};

}  // namespace iphelf::opengl
