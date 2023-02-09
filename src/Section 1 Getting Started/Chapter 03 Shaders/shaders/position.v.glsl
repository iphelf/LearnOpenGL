#version 460 core
layout (location=0) in vec2 aPos;
out vec3 position;

void main() {
    gl_Position=vec4(aPos, 0.0, 1.0);
    position=vec3(aPos, 0.0);
}
