#include <iphelf/opengl/camera.h>

#include <functional>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace iphelf::opengl {

struct Camera::Impl {
  // x: right, y: forward, z: up
  static const glm::vec3 axis_forward;
  static const glm::vec3 axis_right;
  static const glm::vec3 axis_up;
  static constexpr glm::vec3 movement(float forward, float right, float up) {
    return {right, forward, up};
  }
  static constexpr glm::vec3 direction(float yaw, float pitch) {
    float yaw_r{glm::radians(yaw)};
    float pitch_r{glm::radians(pitch)};
    return movement(glm::cos(yaw_r) * glm::cos(pitch_r),
                    glm::sin(yaw_r) * glm::cos(pitch_r), glm::sin(pitch_r));
  }

  // V_world = orientation * V_local (relative to axis_forward)
  const glm::mat3 orientation;  // local to world
  glm::vec3 pos;                // world
  const glm::vec3 up;           // world
  glm::vec3 front;              // world
  float yaw;                    // in degrees (default when without suffix)
  float pitch;                  // in degrees (default when without suffix)
  const float sensitivity;
  Impl(const glm::vec3& pos, const glm::vec3& up, float yaw, float pitch,
       float sensitivity)
      : orientation{std::invoke([&basis_up = up] {
          // try to align with axis_forward
          auto basis_right{glm::normalize(glm::cross(axis_forward, basis_up))};
          auto basis_forward{glm::cross(basis_up, basis_right)};
          return glm::mat3{basis_right, basis_forward, basis_up};
        })},
        pos{pos},
        up{up},
        front{orientation * axis_forward},
        yaw{yaw},
        pitch{pitch},
        sensitivity{sensitivity} {
    recompute_front();
  }
  void recompute_front() { front = orientation * direction(yaw, pitch); }
};
const glm::vec3 Camera::Impl::axis_forward{Camera::Impl::direction(0, 0)};
const glm::vec3 Camera::Impl::axis_right{Camera::Impl::direction(-90, 0)};
const glm::vec3 Camera::Impl::axis_up{Camera::Impl::direction(0, 90)};

Camera::Camera(const glm::vec3& pos, const glm::vec3& up, float yaw,
               float pitch, float sensitivity)
    : self{std::make_unique<Impl>(pos, up, yaw, pitch, sensitivity)} {}

Camera::Camera(Camera&& other) noexcept : self{nullptr} {
  *this = std::move(other);
}

Camera& Camera::operator=(Camera&& other) noexcept {
  std::swap(self, other.self);
  return *this;
}

Camera::~Camera() = default;

void Camera::rotate(float delta_yaw, float delta_pitch, bool constrained) {
  // rotate with respect to current front direction
  delta_yaw *= self->sensitivity;
  delta_pitch *= self->sensitivity;
  self->yaw = std::invoke([yaw = self->yaw + delta_yaw] {
    if (yaw > 720) return yaw - 720;
    if (yaw < -720) return yaw + 720;
    return yaw;
  });
  self->pitch += delta_pitch;
  if (constrained) self->pitch = std::clamp(self->pitch, -89.0f, 89.0f);
  self->recompute_front();
}

void Camera::move(float delta_forward, float delta_right, float delta_up) {
  // move with respect to current front direction
  self->pos += self->front * delta_forward;
  auto right{glm::normalize(glm::cross(self->front, self->up))};
  self->pos += right * delta_right;
  auto up{glm::cross(right, self->front)};
  self->pos += up * delta_up;
}

glm::mat4 Camera::world2view() const {
  return glm::lookAt(self->pos, self->pos + self->front, self->up);
}

}  // namespace iphelf::opengl