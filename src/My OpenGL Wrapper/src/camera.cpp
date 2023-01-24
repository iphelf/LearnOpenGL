#include <iphelf/opengl/camera.h>

#include <algorithm>
#include <functional>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace iphelf::opengl {

struct Camera::Impl {
  // x: right, y: forward, z: up
  static constexpr glm::vec3 movement(float forward, float right, float up) {
    return {right, forward, up};
  }
  static constexpr glm::vec3 direction(float yaw, float pitch) {
    float yaw_r{glm::radians(yaw)};
    float pitch_r{glm::radians(pitch)};
    return movement(glm::cos(yaw_r) * glm::cos(pitch_r),
                    glm::sin(yaw_r) * glm::cos(pitch_r), glm::sin(pitch_r));
  }

  glm::vec3 pos;                // world
  const glm::vec3 up;           // world
  glm::vec3 front;              // world
  const glm::mat3 orientation;  // local to world
  float yaw;                    // in degrees (default when without suffix)
  float pitch;                  // in degrees (default when without suffix)
  const float sensitivity;
  const double default_fov{45.0};
  double fov{default_fov};
  Impl(const glm::vec3& pos, const glm::vec3& up, const glm::vec3& front,
       float yaw, float pitch, float rotation_sensitivity)
      : pos{pos},
        up{glm::normalize(up)},
        front{glm::normalize(glm::cross(this->up, glm::cross(front, up)))},
        orientation{glm::mat3{glm::cross(this->front, this->up), this->front,
                              this->up}},
        yaw{yaw},
        pitch{pitch},
        sensitivity{rotation_sensitivity} {
    recompute_front();
  }
  void recompute_front() { front = orientation * direction(yaw, pitch); }
};

Camera::Camera(const glm::vec3& pos, const glm::vec3& up,
               const glm::vec3& front, float yaw, float pitch,
               float sensitivity)
    : self{std::make_unique<Impl>(pos, up, front, yaw, pitch, sensitivity)} {}

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
  if (constrained) self->pitch = std::clamp(self->pitch, -89.9f, 89.9f);
  self->recompute_front();
}

void Camera::move(float delta_forward, float delta_right) {
  // move with respect to current front direction
  self->pos += self->front * delta_forward;
  auto right{glm::normalize(glm::cross(self->front, self->up))};
  self->pos += right * delta_right;
}

void Camera::ascend(float delta_up) {
  // move with respect to the vertical up
  self->pos += self->up * delta_up;
}

void Camera::zoom(double delta) {
  const double zoom_sensitivity{5};
  self->fov = std::clamp(self->fov - delta * zoom_sensitivity, 5.0, 90.0);
}

void Camera::reset_zoom() { self->fov = self->default_fov; }

glm::mat4 Camera::world2view() const {
  return glm::lookAt(self->pos, self->pos + self->front, self->up);
}

double Camera::fov() const { return self->fov; }

}  // namespace iphelf::opengl