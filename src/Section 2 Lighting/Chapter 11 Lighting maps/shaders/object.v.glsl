#version 460
layout (location = 0) in vec3 v_pos;
layout (location = 1) in vec3 v_normal;
layout (location = 2) in vec2 v_uv;
out vec3 v2f_pos;
out vec3 v2f_normal;
out vec2 v2f_uv;
uniform mat4 u_model2world;
uniform mat3 u_model2world_normalized;
uniform mat4 u_world2view;
uniform mat4 u_view2clip;

void main() {
    vec4 world_pos = u_model2world * vec4(v_pos, 1.0f);
    gl_Position = u_view2clip * u_world2view * world_pos;
    v2f_pos = vec3(world_pos);
    v2f_normal = normalize(u_model2world_normalized * v_normal);
    v2f_uv = v_uv;
}
