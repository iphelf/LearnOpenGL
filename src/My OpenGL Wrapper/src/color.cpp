#include <iphelf/opengl/color.h>

#include <glm/gtx/color_space.hpp>

namespace iphelf::opengl {

Color Color::from_hsv(float hue, float saturation, float value) {
  auto rgb{glm::rgbColor(glm::vec3{hue, saturation, value})};
  return {rgb.r, rgb.g, rgb.b};
}

}  // namespace iphelf::opengl
