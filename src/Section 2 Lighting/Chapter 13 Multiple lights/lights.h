#include <utility>

#pragma once

struct Light {
  const std::string name;
  iphelf::opengl::Color color{1.0f};
  float ambient_intensity{0.1f};
  float diffuse_intensity{1.0f};
  float specular_intensity{0.5f};
  explicit Light(std::string name) : name{std::move(name)} {}
  virtual ~Light() = default;
  virtual void set_uniform(const iphelf::opengl::Program &program) const {
    program.with_uniform(name + ".ambient", color * ambient_intensity);
    program.with_uniform(name + ".diffuse", color * diffuse_intensity);
    program.with_uniform(name + ".specular", color * specular_intensity);
  }
};

struct PointLight : Light {
  glm::vec3 position{0.0f};
  float falloff_constant{1.0f};
  float falloff_linear{0.09f};
  float falloff_quadratic{0.032f};
  PointLight(std::string name, const glm::vec3 &position)
      : Light{std::move(name)}, position{position} {}
  void set_uniform(const iphelf::opengl::Program &program) const override {
    Light::set_uniform(program);
    program.with_uniform(name + ".position", position);
    program.with_uniform(name + ".falloff_constant", falloff_constant);
    program.with_uniform(name + ".falloff_linear", falloff_linear);
    program.with_uniform(name + ".falloff_quadratic", falloff_quadratic);
  }
};

struct DirectionalLight : Light {
  glm::vec3 direction{0.0f, 0.0f, -1.0f};
  explicit DirectionalLight(std::string name) : Light{std::move(name)} {}
  void set_uniform(const iphelf::opengl::Program &program) const override {
    Light::set_uniform(program);
    program.with_uniform(name + ".direction", direction);
  }
};

struct SpotLight : Light {
  glm::vec3 position{0.0f};
  glm::vec3 direction{0.0f, 0.0f, -1.0f};
  float inner{0.96f};
  float outer{0.87f};
  SpotLight(std::string name, const glm::vec3 &position)
      : Light{std::move(name)}, position{position} {}
  void set_uniform(const iphelf::opengl::Program &program) const override {
    Light::set_uniform(program);
    program.with_uniform(name + ".position", position);
    program.with_uniform(name + ".direction", direction);
    program.with_uniform(name + ".inner", inner);
    program.with_uniform(name + ".outer", outer);
  }
};
