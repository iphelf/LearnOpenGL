#version 460
layout (location = 0) in vec2 v_pos;
layout (location = 1) in vec2 v_uv;
layout (location = 2) in vec3 v_color;
out vec2 v2f_uv;
out vec3 v2f_color;

void main() {
    gl_Position=vec4(v_pos, 0.0, 1.0);
    v2f_uv=v_uv;
    v2f_color=v_color;
}
