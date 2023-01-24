#include <iphelf/opengl/color.h>

#include <glm/gtx/color_space.hpp>

namespace iphelf::opengl {

const Color Color::Black{0.0f, 0.0f, 0.0f};
const Color Color::White{1.0f, 1.0f, 1.0f};
const Color Color::DarkGreenBluish{0.2f, 0.3f, 0.3f};
const Color Color::Orange{1.0f, 0.5f, 0.2f};
const Color Color::Yellow{1.0f, 0.0f, 1.0f};

Color Color::from_hsv(float hue, float saturation, float value) {
  auto rgb{glm::rgbColor(glm::vec3{hue, saturation, value})};
  return {rgb.r, rgb.g, rgb.b};
}

}  // namespace iphelf::opengl
