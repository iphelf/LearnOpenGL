#version 460
layout (location = 0) in vec3 v_pos;
layout (location = 1) in vec2 v_uv;
out vec2 v2f_uv;
uniform mat4 u_transform;

void main() {
    gl_Position = u_transform * vec4(v_pos, 1.0);
    v2f_uv = v_uv;
}
