#version 460
in vec3 position;
out vec4 FragColor;

void main() {
    FragColor=vec4(position, 1.0f);
}
