#pragma once

namespace iphelf::opengl {

struct Color {
  float r;
  float g;
  float b;

  explicit Color(float intensity = 0.0f)
      : r{intensity}, g{intensity}, b{intensity} {}

  Color(float red, float green, float blue) : r(red), g(green), b(blue) {}

  Color operator*(float s) const { return {r * s, g * s, b * s}; }

  // Note:
  //   hue \in [0.0f, 360.0f]
  //   saturation, value \in [0.0f, 1.0f]
  static Color from_hsv(float hue, float saturation, float value);
};

namespace Colors {
const Color Black{0.0f, 0.0f, 0.0f};
const Color White{1.0f, 1.0f, 1.0f};
const Color DarkGreenBluish{0.2f, 0.3f, 0.3f};
const Color Orange{1.0f, 0.5f, 0.2f};
const Color Yellow{1.0f, 0.0f, 1.0f};
const Color Dark{0.07f};
};  // namespace Colors

}  // namespace iphelf::opengl
