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
    float falloff_constant;
    float falloff_linear;
    float falloff_quadratic;
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

    vec3 direction_light = normalize(u_light.position - v2f_pos);
    vec3 diffuse = u_light.diffuse * sample_diffuse
        * max(dot(normal, direction_light), 0.0f);

    vec3 direction_reflected = reflect(-direction_light, normal);
    vec3 direction_view = normalize(u_camera_pos - v2f_pos);
    vec3 specular = u_light.specular
        * vec3(texture(u_material.specular, v2f_uv))
        * pow(max(dot(direction_view, direction_reflected), 0.0f),
              u_material.shininess);

    float d = distance(v2f_pos, u_light.position);
    float attenuation = 1.0f / (u_light.falloff_constant + u_light.falloff_linear * d + u_light.falloff_quadratic * d * d);

    f_color = vec4(ambient + (diffuse + specular) * attenuation, 1.0f);
}
