#version 460
in vec3 v2f_pos;
in vec3 v2f_normal;
out vec4 f_color;
uniform vec3 u_light_pos;
uniform vec3 u_camera_pos;
uniform vec3 u_light_color;
uniform vec3 u_object_color;
uniform float u_ambient_strength;
uniform float u_diffuse_strength;
uniform float u_specular_strength;
uniform float u_shininess;

void main() {
    vec3 normal = normalize(v2f_normal);

    float ambient = u_ambient_strength;

    vec3 direction_light = normalize(u_light_pos - v2f_pos);
    float diffuse = u_diffuse_strength
        * max(dot(normal, direction_light), 0.0f);

    vec3 direction_reflected = reflect(-direction_light, normal);
    vec3 direction_view = normalize(u_camera_pos - v2f_pos);
    float specular = u_specular_strength
        * pow(max(dot(direction_view, direction_reflected), 0.0f), u_shininess);

    vec3 light_color = (ambient + diffuse + specular) * u_light_color;

    f_color = vec4(light_color * u_object_color, 1.0f);
}
