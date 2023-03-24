#pragma once

#include <glm/fwd.hpp>
#include <glm/vec3.hpp>
#include <memory>

namespace iphelf::opengl {

class Camera {
  struct Impl;
  std::unique_ptr<Impl> self;

  friend class Application;

  struct Orientation {
    glm::vec3 up{0.0f, 1.0f, 0.0f};
    glm::vec3 front{0.0f, 0.0f, -1.0f};
    float yaw{0.0f};
    float pitch{0.0f};
  };
  struct Sensitivity {
    float rotate{0.2f};
    float move{1.0f};
    float zoom_step{2.0f};
  };
  struct Frustum {
    float fov{45.0f};
    float aspect{800.0f / 600.0f};
    float near{0.1f};
    float far{100.0f};
  };

  Camera(const glm::vec3& position, const Orientation& orientation,
         const Frustum& frustum, const Sensitivity& sensitivity);

 public:
  Camera(Camera&& other) noexcept;
  Camera& operator=(Camera&& other) noexcept;
  ~Camera();

  [[nodiscard]] std::pair<float, float> rotation() const;
  void set_rotation(float yaw, float pitch);
  void rotate(float delta_yaw, float delta_pitch);

  [[nodiscard]] glm::vec3 position() const;
  void set_position(const glm::vec3& position);
  void move(float delta_forward, float delta_right);
  void ascend(float delta_up);

  [[nodiscard]] glm::mat4 model2world() const;

  /// move forward by a zoom_step
  void zoom_in();
  /// move backward by a zoom_step
  void zoom_out();

  [[nodiscard]] glm::mat4 world2view() const;
  [[nodiscard]] glm::mat4 view2clip() const;
  [[nodiscard]] glm::mat4 world2clip() const;
};

}  // namespace iphelf::opengl