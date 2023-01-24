#version 460
layout (location = 0) in vec3 v_pos;
uniform mat4 u_model2world;
uniform mat4 u_world2view;
uniform mat4 u_view2clip;

void main() {
    gl_Position = u_view2clip * u_world2view * u_model2world * vec4(v_pos, 1.0f);
}
