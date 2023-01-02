#version 460
in vec2 v2f_uv;
out vec4 f_color;
uniform sampler2D u_image;

void main() {
    f_color = texture(u_image, v2f_uv);
}
