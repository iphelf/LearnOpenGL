#version 460
out vec4 f_color;
uniform vec3 u_light_color;

void main() {
    f_color = vec4(u_light_color, 1.0f);
}
