#version 460
layout (location = 0) in vec3 v_pos;
layout (location = 1) in vec3 v_normal;
out vec3 v2f_color;
uniform mat4 u_model2world;
uniform mat3 u_model2world_normalized;
uniform mat4 u_world2view;
uniform mat4 u_view2clip;
uniform vec3 u_light_pos;
uniform vec3 u_camera_pos;
uniform vec3 u_light_color;
uniform vec3 u_object_color;
uniform float u_ambient_strength;
uniform float u_diffuse_strength;
uniform float u_specular_strength;
uniform float u_shininess;

void main() {
    vec3 world_pos = vec3(u_model2world * vec4(v_pos, 1.0f));
    gl_Position = u_view2clip * u_world2view * vec4(world_pos, 1.0f);

    vec3 normal = normalize(u_model2world_normalized * v_normal);

    float ambient = u_ambient_strength;

    vec3 direction_light = normalize(u_light_pos - world_pos);
    float diffuse = u_diffuse_strength
        * max(dot(normal, direction_light), 0.0f);

    vec3 direction_reflected = reflect(-direction_light, normal);
    vec3 direction_view = normalize(u_camera_pos - world_pos);
    float specular = u_specular_strength
        * pow(max(dot(direction_view, direction_reflected), 0.0f), u_shininess);

    vec3 light_color = (ambient + diffuse + specular) * u_light_color;
    v2f_color = light_color * u_object_color;
}
