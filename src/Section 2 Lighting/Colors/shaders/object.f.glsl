#version 460
out vec4 f_color;
uniform vec3 u_light_color;
uniform vec3 u_object_color;

void main() {
    f_color = vec4(u_light_color * u_object_color, 1.0f);
}
