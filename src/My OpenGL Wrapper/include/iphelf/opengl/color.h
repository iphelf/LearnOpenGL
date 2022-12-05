#pragma once

namespace iphelf::opengl {

struct Color {
  float r;
  float g;
  float b;
  float a;

  explicit Color(float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 1.0f)
      : r(r), g(g), b(b), a(a) {}

  static const Color Black;
  static const Color White;
  static const Color DarkGreenBluish;
  static const Color Orange;
};

}
