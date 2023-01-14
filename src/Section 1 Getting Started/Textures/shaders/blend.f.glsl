#version 460
in vec2 v2f_uv;
in vec3 v2f_color;
out vec4 f_color;
uniform sampler2D u_image_container;
uniform sampler2D u_image_face;
uniform float u_scale;
uniform float u_offset_u;
uniform float u_offset_v;
uniform float u_intensity;

void main() {
  vec2 transformed_uv = (v2f_uv + vec2(u_offset_u, u_offset_v)) * u_scale + (1.0f - u_scale) * 0.5f;
  vec4 image_color = mix(
    texture(u_image_container, v2f_uv),
    texture(u_image_face, transformed_uv),
    0.5
  );
  vec4 tint_color = vec4(v2f_color, 1.0f);
  f_color = mix(image_color, tint_color, u_intensity);
}
