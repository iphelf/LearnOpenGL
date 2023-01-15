#version 460
layout (location = 0) in vec3 v_pos;
layout (location = 1) in vec2 v_uv;
out vec2 v2f_uv;
uniform mat4 u_model2world;
uniform mat4 u_world2view;
uniform mat4 u_view2clip;

void main() {
    gl_Position = u_view2clip * u_world2view * u_model2world * vec4(v_pos, 1.0f);
    v2f_uv = v_uv;
}
