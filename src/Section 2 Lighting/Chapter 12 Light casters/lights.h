#pragma once

struct Light {
  iphelf::opengl::Program program;
  iphelf::opengl::Color color{1.0f};
  float ambient_intensity{0.1f};
  float diffuse_intensity{1.0f};
  float specular_intensity{0.5f};
  explicit Light(iphelf::opengl::Program &&program)
      : program{std::move(program)} {}
  virtual ~Light() = default;
  virtual void set_uniform() const {
    program.with_uniform("u_light.ambient", color * ambient_intensity);
    program.with_uniform("u_light.diffuse", color * diffuse_intensity);
    program.with_uniform("u_light.specular", color * specular_intensity);
  }
};

struct PointLight : Light {
  glm::vec3 position{0.0f};
  float falloff_constant{1.0f};
  float falloff_linear{0.09f};
  float falloff_quadratic{0.032f};
  explicit PointLight(iphelf::opengl::Program &&program)
      : Light{std::move(program)} {}
  void set_uniform() const override {
    Light::set_uniform();
    program.with_uniform("u_light.position", position);
    program.with_uniform("u_light.falloff_constant", falloff_constant);
    program.with_uniform("u_light.falloff_linear", falloff_linear);
    program.with_uniform("u_light.falloff_quadratic", falloff_quadratic);
  }
};

struct DirectionalLight : Light {
  glm::vec3 direction{0.0f, 0.0f, -1.0f};
  explicit DirectionalLight(iphelf::opengl::Program &&program)
      : Light{std::move(program)} {}
  void set_uniform() const override {
    Light::set_uniform();
    program.with_uniform("u_light.direction", direction);
  }
};

struct SpotLight : Light {
  glm::vec3 position{0.0f};
  glm::vec3 direction{0.0f, 0.0f, -1.0f};
  float inner{0.96f};
  float outer{0.87f};
  explicit SpotLight(iphelf::opengl::Program &&program)
      : Light{std::move(program)} {}
  void set_uniform() const override {
    Light::set_uniform();
    program.with_uniform("u_light.position", position);
    program.with_uniform("u_light.direction", direction);
    program.with_uniform("u_light.inner", inner);
    program.with_uniform("u_light.outer", outer);
  }
};
