#version 460

in vec3 v2f_pos;
in vec3 v2f_normal;
in vec2 v2f_uv;

out vec4 f_color;

uniform vec3 u_camera_pos;
struct Light{
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform Light u_light;
struct Material{
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};
uniform Material u_material;

void main() {
    vec3 normal = normalize(v2f_normal);
    vec3 sample_diffuse = vec3(texture(u_material.diffuse, v2f_uv));

    vec3 ambient = u_light.ambient * sample_diffuse;

    vec3 diffuse = u_light.diffuse * sample_diffuse
        * max(dot(normal, -u_light.direction), 0.0f);

    vec3 direction_reflected = reflect(u_light.direction, normal);
    vec3 direction_view = normalize(u_camera_pos - v2f_pos);
    vec3 specular = u_light.specular
        * vec3(texture(u_material.specular, v2f_uv))
        * pow(max(dot(direction_view, direction_reflected), 0.0f),
              u_material.shininess);

    f_color = vec4(ambient + diffuse + specular, 1.0f);
}
