#include <iphelf/opengl/camera.h>

#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace iphelf::opengl {

struct Camera::Impl {
  // x: right, y: up, z: -forward

  glm::vec3 position;           // world
  const glm::vec3 up;           // world
  glm::vec3 right;              // world
  glm::vec3 front;              // world
  const glm::mat3 orientation;  // local to world rotation matrix
  float yaw{0.0f};              // in degrees
  float pitch{0.0f};            // in degrees
  const glm::mat4 view2clip;
  const Camera::Sensitivity sensitivity;
  Impl(const glm::vec3& position, const Orientation& orientation,
       const Frustum& frustum, const Sensitivity& sensitivity)
      : position{position},
        up{glm::normalize(orientation.up)},
        right{glm::normalize(glm::cross(orientation.front, up))},
        front{glm::cross(up, right)},
        orientation{glm::mat3{right, up, -front}},
        view2clip{glm::perspective(
            glm::radians(std::clamp(frustum.fov, 0.01f, 180.0f)),
            frustum.aspect, frustum.near, frustum.far)},
        sensitivity{sensitivity} {
    set_yaw_pitch(orientation.yaw, orientation.pitch);
  }
  void set_yaw_pitch(float new_yaw, float new_pitch) {
    yaw = std::fmod(new_yaw, 720.0f);
    pitch = std::clamp(new_pitch, -89.9f, 89.9f);
    float yaw_r{glm::radians(yaw)};
    float pitch_r{glm::radians(pitch)};
    front = orientation * glm::vec3{glm::sin(yaw_r) * glm::cos(pitch_r),
                                    glm::sin(pitch_r),
                                    -glm::cos(yaw_r) * glm::cos(pitch_r)};
  }
};

Camera::Camera(const glm::vec3& position,
               const Camera::Orientation& orientation, const Frustum& frustum,
               const Camera::Sensitivity& sensitivity)
    : self{std::make_unique<Impl>(position, orientation, frustum,
                                  sensitivity)} {}

Camera::Camera(Camera&& other) noexcept : self{nullptr} {
  *this = std::move(other);
}

Camera& Camera::operator=(Camera&& other) noexcept {
  std::swap(self, other.self);
  return *this;
}

Camera::~Camera() = default;

std::pair<float, float> Camera::rotation() const {
  return {self->yaw, self->pitch};
}

void Camera::set_rotation(float yaw, float pitch) {
  self->set_yaw_pitch(yaw, pitch);
}

void Camera::rotate(float delta_yaw, float delta_pitch) {
  self->set_yaw_pitch(self->yaw + delta_yaw * self->sensitivity.rotate,
                      self->pitch + delta_pitch * self->sensitivity.rotate);
}

glm::vec3 Camera::position() const { return self->position; }

void Camera::set_position(const glm::vec3& position) {
  self->position = position;
}

void Camera::move(float delta_forward, float delta_right) {
  self->position += self->front * delta_forward * self->sensitivity.move;
  self->position += glm::normalize(glm::cross(self->front, self->up)) *
                    delta_right * self->sensitivity.move;
}

void Camera::ascend(float delta_up) {
  self->position += self->up * delta_up * self->sensitivity.move;
}

void Camera::zoom_in() {
  self->position +=
      self->front * self->sensitivity.zoom_step * self->sensitivity.move;
}

void Camera::zoom_out() {
  self->position -=
      self->front * self->sensitivity.zoom_step * self->sensitivity.move;
}

glm::mat4 Camera::world2view() const {
  // measure which is better: compute on input, or compute at each frame
  return glm::lookAt(self->position, self->position + self->front, self->up);
}

glm::mat4 Camera::view2clip() const { return self->view2clip; }

glm::mat4 Camera::world2clip() const { return world2view() * self->view2clip; }

}  // namespace iphelf::opengl