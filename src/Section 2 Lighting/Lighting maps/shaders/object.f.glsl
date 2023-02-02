#version 460

in vec3 v2f_pos;
in vec3 v2f_normal;
in vec2 v2f_uv;

out vec4 f_color;

uniform vec3 u_camera_pos;
struct Light{
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform Light u_light;
struct Material{
    sampler2D emission;
    float emission_intensity;
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};
uniform Material u_material;

void main() {
    vec3 normal = normalize(v2f_normal);
    vec3 fraction_diffuse = vec3(texture(u_material.diffuse, v2f_uv));

    vec3 emission = u_material.emission_intensity
        * vec3(texture(u_material.emission, v2f_uv * 1.2f - 0.1f));

    vec3 ambient = u_light.ambient * fraction_diffuse;

    vec3 direction_light = normalize(u_light.position - v2f_pos);
    vec3 diffuse = u_light.diffuse * fraction_diffuse
    * max(dot(normal, direction_light), 0.0f);

    vec3 direction_reflected = reflect(-direction_light, normal);
    vec3 direction_view = normalize(u_camera_pos - v2f_pos);
    vec3 specular = u_light.specular
    * vec3(texture(u_material.specular, v2f_uv))
    * pow(max(dot(direction_view, direction_reflected), 0.0f),
    u_material.shininess);

    f_color = vec4(emission + ambient + diffuse + specular, 1.0f);
}
