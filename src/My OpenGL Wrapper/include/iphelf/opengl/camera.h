#pragma once

#include <glm/fwd.hpp>
#include <memory>

namespace iphelf::opengl {

class Camera {
  struct Impl;
  std::unique_ptr<Impl> self;

  friend class Application;

  Camera(const glm::vec3 &pos, const glm::vec3 &up, float yaw, float pitch,
         float sensitivity);

 public:
  Camera(Camera &&other) noexcept;
  Camera &operator=(Camera &&other) noexcept;
  ~Camera();

  void rotate(float delta_yaw, float delta_pitch, bool constrained = true);
  void move(float delta_forward, float delta_right, float delta_up);
  [[nodiscard]] glm::mat4 world2view() const;
};

}  // namespace iphelf::opengl